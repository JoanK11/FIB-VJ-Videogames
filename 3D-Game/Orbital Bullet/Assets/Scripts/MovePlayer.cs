using System.Collections;
using System.Collections.Generic;
using UnityEngine;



public class MovePlayer : MonoBehaviour {
    /* -- Player Movement -- */
    float rotationSpeed, jumpSpeed, gravity;
    Vector3 Center;
    const float maxRotationSpeed = 70.0f;

    /* -- Double Jump -- */
    int jumpCount;
    const int maxJumpCount = 2;
    Vector3 startDirection;
    float speedY;
    float changingLevelTime;

    /* -- Player States -- */
    public enum PlayerStates { Normal, ChangingLevel, ChangingRing, Invincible };
    PlayerStates State;

    /* -- Shooting -- */
    float timeToRestartShoot;
    float restartTime;
    public GameObject prefab;
    bool reloading;
    int num;

    void Start() {
        // Store starting direction of the player with respect to the axis of the level
        startDirection = transform.position - transform.parent.position;
        startDirection.y = 0.0f;
        startDirection.Normalize();
        speedY = 0;
        changingLevelTime = 0;

        State = PlayerStates.Normal;

        /* -- Player Movement -- */
        rotationSpeed = 0;
        jumpSpeed = 8.25f;
        gravity = 25;
        Center = new Vector3(0, 0, 0);

        /* -- Double Jump -- */
        jumpCount = 0;

        /* -- Shooting -- */
        timeToRestartShoot = 0.25f;
        restartTime = 0;
        reloading = false;
        num = 0;
    }

    void FixedUpdate() {
        CharacterController charControl = GetComponent<CharacterController>();
        
        bool canMove = true;
        if (State == PlayerStates.ChangingRing ||
            State == PlayerStates.ChangingLevel) {
            canMove = false;
        }

        Vector3 position;

        float angle;
        Vector3 direction, target;

        position = transform.position;
        angle = rotationSpeed * Time.deltaTime;
        direction = position - Center;

        /* -- Horizontal Movement -- */
        if ((Input.GetKey(KeyCode.A) || Input.GetKey(KeyCode.D)) && canMove) {

            if (Input.GetKey(KeyCode.A)) {
                target = Center + Quaternion.AngleAxis(angle, Vector3.up) * direction;
                if (charControl.Move(target - position) != CollisionFlags.None) {
                    transform.position = position;
                    Physics.SyncTransforms();
                }
            }
            if (Input.GetKey(KeyCode.D)) {
                target = Center + Quaternion.AngleAxis(-angle, Vector3.up) * direction;
                if (charControl.Move(target - position) != CollisionFlags.None) {
                    transform.position = position;
                    Physics.SyncTransforms();
                }
            }
            // Increment Character Speed
            rotationSpeed += 2.0f;
            if (rotationSpeed > maxRotationSpeed) rotationSpeed = maxRotationSpeed;
        }
        else {
            rotationSpeed -= 5.0f;
            if (rotationSpeed < 0) rotationSpeed = 0;
        }

        /* -- Shooting -- */
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

        if (Input.GetKey(KeyCode.K) && !reloading) {
            reloading = true;
            restartTime = 0.0f;
            Vector3 bulletPos = Center + Quaternion.AngleAxis(-angle - 15.0f, Vector3.up) * direction;
            bulletPos.y = transform.position.y;
            Debug.Log(bulletPos);
            GameObject newObject = Instantiate(prefab, bulletPos, transform.rotation, transform.parent);
            newObject.name = "bala" + (++num);
        }

        if (reloading) {
            restartTime += Time.deltaTime;
            if (restartTime >= timeToRestartShoot) {
                reloading = false;
            }
        }

        /* -- Vertical Movement && Double Jump --
        if (IsGrounded() && jumpCount > 0) {
            jumpCount = 0;
        }
        if (State != PlayerStates.ChangingRing) {
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
            if ((flags & CollisionFlags.Below) != 0) {
                speedY = 0;
            }
        }*/
        // Apply up-down movement
        if (State != PlayerStates.ChangingRing) {
            position = transform.position;
            if (charControl.Move(speedY * Time.deltaTime * Vector3.up) != CollisionFlags.None) {
                transform.position = position;
                Physics.SyncTransforms();
            }
            if (charControl.isGrounded) {
                if (Input.GetKey(KeyCode.W))
                    speedY = jumpSpeed;
            }
            else
                speedY -= gravity * Time.deltaTime;
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
            Debug.Log(name + ": Is Grounded " + checkDistance);
            return true;
        }
        Debug.Log(name + ": Is not Grounded " + checkDistance);
        return false;
    }

    public void JumpNextLevel() {
        speedY = 20.5f;
        State = PlayerStates.ChangingLevel;
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

    public void ChangeRing(Vector3 targetPosition) {
        State = PlayerStates.ChangingRing;
        StartCoroutine(MovePlayerToPosition(transform.position, targetPosition, 0.6f));
    }

    public void ChangeCylinder(Vector3 targetPosition) {
        State = PlayerStates.ChangingRing;
        StartCoroutine(MovePlayerToPosition(transform.position, targetPosition, 3.0f));
        Center = new Vector3(50, 0, 0);
        FollowPlayer cameraScript = GameObject.FindGameObjectWithTag("MainCamera").GetComponent<FollowPlayer>();
        StartCoroutine(cameraScript.ChangeCylinder(3.5f, 50.0f));
    }
}