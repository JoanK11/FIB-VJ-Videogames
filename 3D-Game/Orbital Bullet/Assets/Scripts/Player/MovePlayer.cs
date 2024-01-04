using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
using System;


public class MovePlayer : MonoBehaviour {
    /* -- Player Movement -- */
    float rotationSpeed, jumpSpeed, gravity;
    Vector3 Center;
    const float maxRotationSpeed = 70.0f;

    /* -- Double Jump -- */
    //int jumpCount;
    const int maxJumpCount = 2;
    Vector3 startDirection;
    float speedY;

    /* -- Player States -- */
    public enum PlayerStates { Normal, ChangingLevel, ChangingRing, Invulnerable };
    PlayerStates State;

    /* -- Dash -- */
    float oneOrientation;
    const float TimeDashOcurr = 1.6f;
    const float VelocityOfDashing = 25.0f / TimeDashOcurr;
    const float RotationHimself = 360.0f / TimeDashOcurr;
    bool isDashing;
    float TimeDashing;

    /* -- Shooting -- */
    public GameObject prefab;
    bool reloading;

    /* -- Weapons -- */
    WeaponBase[] weapons;
    int index;
    WeaponBase currentWeapon;

    /* -- Animation -- */
    CharacterController charControl;
    Animator anim;

    /* -- Health -- */
    const float maxHealth = 120.0f;
    float health;
    HealthBar healthBar;

    /* -- Audio -- */
    PlayerAudio playerAudio;

    /* -- UI -- */
    public GameOver gameOver;

    /* --  Ammo -- */
    const int maxAmmo = 30;
    const int startAmmo = 10;
    int ammo;
    CurrentAmmo text;

    public Vector3 GetCenter() {
        return Center;
    }

    public Vector3 GetStartDirection() {
        return startDirection;
    }

    void SetupWeapons() { 
        weapons = GetComponentsInChildren<WeaponBase>(true);
        foreach (WeaponBase weapon in weapons) {
            weapon.gameObject.SetActive(false);
        }
        weapons[0].gameObject.SetActive(true);
        index = 0;
        currentWeapon = weapons[index];
    }

    public void EnableWeapon(int weaponIndex) {
        // weaponIndex == 0 -> Long Range
        // weaponIndex == 1 -> Short Range
        /* -- Jeremy mira de completar esta función, la llama ChestWeapon con weaponIndex == 1,
         * para activar el arco de corto alcance cuando lo coge del cofre -- */
    }

    void CheckSelectionWeapon() {
        if (Input.GetKeyDown(KeyCode.L)) {
            weapons[index].gameObject.SetActive(false);
            index = (index + 1) % weapons.Length;

            weapons[index].gameObject.SetActive(true);
            currentWeapon = weapons[index];
        }
    }

    public void SetAmmo(int ammo) {
        this.ammo = Math.Min(ammo, maxAmmo);
        text.SetAmmo(this.ammo);
    }

    public void IncreaseAmmo(int x) {
        ammo = Math.Min(ammo + x, maxAmmo);
        text.SetAmmo(ammo);
    }

    void Start() {
        // Store starting direction of the player with respect to the axis of the level
        startDirection = transform.position - transform.parent.position;
        startDirection.y = 0.0f;
        startDirection.Normalize();
        speedY = 0;

        State = PlayerStates.Normal;

        /* -- Player Movement -- */
        rotationSpeed = 0;
        jumpSpeed = 8.5f;
        gravity = 25;
        Center = new Vector3(0, 0, 0);

        /* -- Double Jump -- */
        //jumpCount = 0;

        /* -- Shooting -- */
    
        reloading = false;

        /* -- Dashing Time -- */
        TimeDashing = 0.0f;
        isDashing = false;

        oneOrientation = -1.0f;
        charControl = GetComponent<CharacterController>();
        anim = GetComponent<Animator>();

        /* -- Weapons -- */
        SetupWeapons();

        health = maxHealth;
        GameObject tmp = GameObject.Find("Health Bar");
        healthBar = tmp.GetComponentInChildren<HealthBar>();
        
        healthBar.SetMaxHealth(maxHealth);

        /* -- Audio -- */
        playerAudio = GetComponent<PlayerAudio>();

        /* -- Ammo -- */
        text = GameObject.Find("CurrentAmmo").GetComponent<CurrentAmmo>();
        SetAmmo(startAmmo);
    }

    public void TakeDamage(float damageAmount) {
        if (State != PlayerStates.Normal) return;
        if (isDashing) return;
        health -= damageAmount;
        playerAudio.PlayDamageSound();
        healthBar.SetHealth(health);

        if (health <= 0) {
            playerAudio.PlayDieSound();
            gameOver.OnPlayerDeath();
        }
    }

    public void IncreaseHealth(int x) {
        health = Math.Min(health + x, maxHealth);
        healthBar.SetHealth(health);
    }

    void Update() {
        /* -- CheckingSelectionOfWeapon -- */
        CheckSelectionWeapon();
        /*-- Cheat of reloading -- */
        if (Input.GetKeyDown(KeyCode.G)) SetAmmo(maxAmmo);

        /* -- CheckingDash -- */
        CheckDashing(charControl);
    }

    void Shoot() {
        Vector3 currentDirection = transform.position - Center;
        reloading = false;
        Vector3 bulletPos = Center + Quaternion.AngleAxis(oneOrientation * 5.0f, Vector3.up) * currentDirection;
        bulletPos.y = transform.position.y;
        currentWeapon.Shoot(bulletPos, transform.rotation, transform.parent, oneOrientation);
        SetAmmo(ammo - 1);
    }

    // Update is called once per frame
    void FixedUpdate() {
        bool canMove = true;
        if (State == PlayerStates.ChangingRing ||
            State == PlayerStates.ChangingLevel) {
            canMove = false;
        }

        /* -- Left-Right Movement -- */
        Vector3 position = transform.position;
        float angle = rotationSpeed * Time.deltaTime;
        Vector3 direction = position - Center;
        Vector3 target;

        if (!isDashing && canMove) {
          /* -- Horizontal Movement -- */
          if (Input.GetKey(KeyCode.A) || Input.GetKey(KeyCode.D)) {

              if (Input.GetKey(KeyCode.A)) {
                  target = Center + Quaternion.AngleAxis(angle, Vector3.up) * direction;
                  if (charControl.Move(target - position) != CollisionFlags.None) {
                      transform.position = position;
                      Physics.SyncTransforms();
                  }
                  anim.SetBool("Moving", true);
                  oneOrientation = 1.0f;
              }
              if (Input.GetKey(KeyCode.D)) {
                  target = Center + Quaternion.AngleAxis(-angle, Vector3.up) * direction;
                  if (charControl.Move(target - position) != CollisionFlags.None) {
                      transform.position = position;
                      Physics.SyncTransforms();
                  }
                  anim.SetBool("Moving", true);
                  oneOrientation = -1.0f;
              }
              // Increment Character Speed
              rotationSpeed += 5.0f;
              if (rotationSpeed > maxRotationSpeed) rotationSpeed = maxRotationSpeed;
          }
          else {
              rotationSpeed -= 3.0f;
              if (rotationSpeed < 0) rotationSpeed = 0;
              anim.SetBool("Moving", false);
          }
        }

        /* -- Correction of Player Movement -- */
        Vector3 currentDirection = transform.position - Center;
        currentDirection.y = 0.0f;
        currentDirection.Normalize();

        // Change orientation of player accordingly
        Quaternion orientation;
        if ((startDirection - currentDirection).magnitude < 1e-3)
            orientation = Quaternion.AngleAxis(0.0f, Vector3.up);
        else if ((startDirection + currentDirection).magnitude < 1e-3)
            orientation = Quaternion.AngleAxis(180.0f, Vector3.up);
        else
            orientation = Quaternion.FromToRotation(startDirection, currentDirection);
        transform.rotation = orientation;
        if (oneOrientation == 1.0f) {
            transform.rotation *= Quaternion.Euler(0, 180.0f, 0);
        }

        /* -- Shooting -- */
        if (Input.GetKey(KeyCode.K) && !isDashing && !reloading) {
            if (ammo > 0) {
                reloading = true;
                playerAudio.PlayAttackSound();
                anim.SetTrigger("Shoot");
                Invoke("Shoot", 0.7f);
            }
            else {
                playerAudio.PlayNoBulletsSound();
            }
        }

        
        // Apply up-down movement
        if (State != PlayerStates.ChangingRing && !isDashing) {
            position = transform.position;
            if (charControl.Move(speedY * Time.deltaTime * Vector3.up) != CollisionFlags.None) {
                transform.position = position;
                Physics.SyncTransforms();
            }
            if (charControl.isGrounded) {
                if (Input.GetKey(KeyCode.W)) {
                    speedY = jumpSpeed;
                    playerAudio.PlayJumpSound();
                }
            }
            else
                speedY -= gravity * Time.deltaTime;
        }
    }

    private bool IsGrounded() {
        CharacterController charControl = GetComponent<CharacterController>();

        // Calculate the bottom position of the character controller
        Vector3 rayStart = transform.position - new Vector3(0, charControl.height / 5.5f, 0);
        float checkDistance = charControl.skinWidth + 0.05f; // Small distance plus skinWidth
        Debug.DrawRay(rayStart, Vector3.down * checkDistance, Color.red, 1.0f);

        RaycastHit hit;
        if (Physics.Raycast(rayStart, Vector3.down, out hit, checkDistance)) {
            return true;
        }
        return false;
    }

    public void JumpNextLevel() {
        speedY = 20.5f;
        State = PlayerStates.ChangingLevel;
        playerAudio.PlayTeleportSound();
        Debug.Log(name + " jumped to the next level.");
    }

    public void ArrivedNextLevel() {
        State = PlayerStates.Normal;
    }

    private IEnumerator MovePlayerToPosition(Vector3 startPosition, Vector3 finalPosition, float duration) {
        float elapsed = 0.0f;
        float peakHeight = 1f; // Adjust for desired peak height

        while (elapsed < duration) {
            elapsed += Time.deltaTime;
            float progress = elapsed / duration;

            // Interpolate x and z position linearly
            float xPosition = Mathf.Lerp(startPosition.x, finalPosition.x, progress);
            float zPosition = Mathf.Lerp(startPosition.z, finalPosition.z, progress);

            // Calculate y position using a parabolic formula for a jump
            float yPosition = Mathf.Lerp(startPosition.y, finalPosition.y, progress) + peakHeight * Mathf.Sin(Mathf.PI * progress);

            // Update the position
            transform.position = new Vector3(xPosition, yPosition, zPosition);

            yield return null; // Wait until the next frame
        }

        // Ensure the player is exactly at the final position after the jump
        //transform.position = finalPosition;
        State = PlayerStates.Normal;
    }

    public void ChangeRing(Vector3 targetPosition, float duration) {
        State = PlayerStates.ChangingRing;
        playerAudio.PlayRingChangeSound();
        StartCoroutine(MovePlayerToPosition(transform.position, targetPosition, duration));
    }

    public void ChangeCylinder(Vector3 targetPosition) {
        State = PlayerStates.ChangingRing;
        StartCoroutine(MovePlayerToPosition(transform.position, targetPosition, 3.0f));
        Center = new Vector3(50, 0, 0);
        FollowPlayer cameraScript = GameObject.FindGameObjectWithTag("MainCamera").GetComponent<FollowPlayer>();
        StartCoroutine(cameraScript.ChangeCylinder(3.5f, 50.0f));
    }

    private void CheckDashing(CharacterController charControl) {
        if (Input.GetKeyDown(KeyCode.LeftShift)) {
            isDashing = true;
            TimeDashing = 0.0f;
            
            anim.SetTrigger("Roll");
        }

        if (isDashing) {
            float time = Time.deltaTime;
            TimeDashing += time;
            Vector3 position = transform.position;
            float angle = oneOrientation * VelocityOfDashing * time;
            Vector3 direction = position - Center;
            
            Vector3 target = Center + Quaternion.AngleAxis(angle, Vector3.up) * direction;
            if (charControl.Move(target - position) != CollisionFlags.None) {
                Debug.Log("He entrado");
                transform.position = position;
                Physics.SyncTransforms();
            }
           
            if (TimeDashing > TimeDashOcurr) {
                isDashing = false;
                
            }
        }
    }

    public void TeleportTo(Vector3 newPosition, Vector3 newCenter) {
        CharacterController charControl = GetComponent<CharacterController>();
        transform.position = newPosition;
        Center = newCenter;
        State = PlayerStates.Normal;
    }
}
