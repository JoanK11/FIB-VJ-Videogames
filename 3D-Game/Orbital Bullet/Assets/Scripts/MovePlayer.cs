using System.Collections;
using System.Collections.Generic;
using UnityEngine;



public class MovePlayer : MonoBehaviour {
    /* -- Player Movement -- */
    float rotationSpeed, jumpSpeed, gravity;

    /* -- Double Jump -- */
    int jumpCount;
    const int maxJumpCount = 2;

    Vector3 startDirection;
    float speedY;
    float changingLevelTime;
    float oneOrientation;
    public enum PlayerState { Normal, ChangingLevel, ChangingRing, Invincible };
    PlayerState State;
    const float TimeDashOcurr = 1.5f;
    const float VelocityOfDashing = 50.0f / TimeDashOcurr;
    const float RotationHimself = 360.0f / TimeDashOcurr;
    bool isDashing;
    float TimeDashing;
    /* -- Shooting -- */
    float timeToRestartShoot;
    float restartTime;
    public GameObject prefab;
    bool reloading;
    int num;


    CharacterController charControl;
    Animator anim;


    // Start is called before the first frame update
    void Start() {
        // Store starting direction of the player with respect to the axis of the level
        startDirection = transform.position - transform.parent.position;
        startDirection.y = 0.0f;
        startDirection.Normalize();
        speedY = 0;
        changingLevelTime = 0;

        State = PlayerState.Normal;
      
        /* -- Player Movement -- */
        rotationSpeed = 70;
        jumpSpeed = 8.25f;
        gravity = 25;

        /* -- Double Jump -- */
        jumpCount = 0;

        /* -- Shooting -- */
        timeToRestartShoot = 0.25f;
        restartTime = 0;
        reloading = false;
        num = 0;

        /* -- Dashing Time -- */
        TimeDashing = 0.0f;
        isDashing = false;

        oneOrientation = -1.0f;
        charControl = GetComponent<CharacterController>();
        anim = GetComponent<Animator>();
    }

    // Update is called once per frame
    void FixedUpdate() {
        

        bool canMove = true;
        if (State == PlayerState.ChangingLevel) {
            changingLevelTime += Time.deltaTime;
            if (IsGrounded() && changingLevelTime > 0.5f) {
                State = PlayerState.Normal;
                changingLevelTime = 0;
            }
            else canMove = false;
        }
        else if (State == PlayerState.ChangingRing) {
            canMove = false;
        }

        

        /* -- Left-Right Movement -- */

        Vector3 position = transform.position;
        float angle = rotationSpeed * Time.deltaTime;
        Vector3 direction = position - transform.parent.position;

        if (! isDashing ) {
            

            if ((Input.GetKey(KeyCode.A) || Input.GetKey(KeyCode.D)) && canMove )
            {

                if (Input.GetKey(KeyCode.A))
                {
                    Vector3 target = transform.parent.position + Quaternion.AngleAxis(angle, Vector3.up) * direction;
                    if (charControl.Move(target - position) != CollisionFlags.None)
                    {
                        transform.position = position;
                        Physics.SyncTransforms();
                        
                    }
                    
                    anim.SetBool("Moving", true);
                    
                    oneOrientation = 1.0f;

                }
                if (Input.GetKey(KeyCode.D))
                {
                    Vector3 target = transform.parent.position + Quaternion.AngleAxis(-angle, Vector3.up) * direction;
                    if (charControl.Move(target - position) != CollisionFlags.None)
                    {
                        transform.position = position;
                        Physics.SyncTransforms(); 
                        
                    }
                    
                        anim.SetBool("Moving", true);
                    
                    oneOrientation = -1.0f;
                }
            }
            else anim.SetBool("Moving", false);
        }

        /* -- Correction of Player Movement -- */
        Vector3 currentDirection = transform.position - transform.parent.position;
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
        if(oneOrientation == 1.0f) transform.rotation *= Quaternion.Euler(0f, 180f,0f);


        /* -- CheckingDash -- */
        CheckDashing(charControl);


        /* -- Shooting -- */
        if (Input.GetKey(KeyCode.K) && !reloading) {
           currentDirection = transform.position - transform.parent.position;
            reloading = true;
            restartTime = 0.0f;
            Vector3 bulletPos = transform.parent.position + Quaternion.AngleAxis(oneOrientation* 5.0f, Vector3.up) * currentDirection;
            bulletPos.y = transform.position.y;
            Debug.Log(bulletPos);
            GameObject newObject = Instantiate(prefab, bulletPos, transform.rotation, transform.parent);

            BalaPlayer bEnemy = newObject.AddComponent<BalaPlayer>();
            Debug.Log("orientation: " + oneOrientation);
            bEnemy.init();
            bEnemy.setOrientation(oneOrientation);
            newObject.name = "bala" + (++num);
            anim.SetTrigger("Shoot");
        }
        if (reloading) {
            restartTime += Time.deltaTime;
            if (restartTime >= timeToRestartShoot) {
                reloading = false;
            }
        }

        /* -- Vertical Movement && Double Jump -- */
        if (IsGrounded()) {
            jumpCount = 0;
        }
        if (State != PlayerState.ChangingRing) {
       
            if (Input.GetKeyDown(KeyCode.W) && jumpCount < maxJumpCount) {
                speedY = jumpSpeed;
                jumpCount++;
            
            }
            else if (!IsGrounded()) {
                speedY -= gravity * Time.deltaTime;
            }

            position = transform.position;
            Vector3 moveVector = new Vector3(0, speedY, 0) * Time.deltaTime;
            CollisionFlags flags = charControl.Move(moveVector);
            if (flags != CollisionFlags.Below) {
                speedY = 0;
            
            }
        }
    }

    private bool IsGrounded() {
        CharacterController charControl = GetComponent<CharacterController>();

        // Calculate the bottom position of the character controller
        Vector3 rayStart = transform.position - new Vector3(0, charControl.height / 4, 0);
        float checkDistance = charControl.skinWidth + 0.1f; // Small distance plus skinWidth
        Debug.DrawRay(rayStart, Vector3.down * checkDistance, Color.red, 1.0f);

        RaycastHit hit;
        if (Physics.Raycast(rayStart, Vector3.down, out hit, checkDistance)) {
           // Debug.Log(name + ": Is Grounded " + checkDistance);
            return true;
        }
       // Debug.Log(name + ": Is not Grounded " + checkDistance);
        return false;
    }

    public void JumpNextLevel() {
        if (IsGrounded()) {
            speedY = 20.0f;
            GameObject world = GameObject.Find("World");
            world.GetComponent<World>().NextLevel();
            State = PlayerState.ChangingLevel;
            //Debug.Log(name + ": Jumped to the next level.");
        }
    }

    private IEnumerator MovePlayerToPosition(Vector3 startPosition, Vector3 finalPosition) {
        float duration = 0.6f; // Duration of the jump
        float elapsed = 0.0f;
        float peakHeight = 1.5f; // Adjust for desired peak height

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
        State = PlayerState.Normal;
    }

    public void ChangeRing(Vector3 targetPosition) {
        State = PlayerState.ChangingRing;
        StartCoroutine(MovePlayerToPosition(transform.position, targetPosition));
    }

    private void CheckDashing(CharacterController charControl) {
        if (Input.GetKey(KeyCode.LeftShift)) {
            isDashing = true;
            TimeDashing = 0.0f;
        }
        if (isDashing) {
            float time = Time.deltaTime;
            TimeDashing += time;
            Vector3 position = transform.position;
            float angle = oneOrientation *VelocityOfDashing * time ;
            Vector3 direction = position - transform.parent.position;
            Vector3 target = transform.parent.position + Quaternion.AngleAxis(angle, Vector3.up) * direction;
            if (charControl.Move(target - position) != CollisionFlags.None)
            {
                transform.position = position;
                Physics.SyncTransforms();

            }
            float bodyRotation = - RotationHimself * TimeDashing;

            Quaternion rotation = Quaternion.Euler(0f, 0f, bodyRotation);
            transform.rotation *= rotation;
        
            if (TimeDashing > TimeDashOcurr) {
                isDashing = false;
            }
        }
    }
}