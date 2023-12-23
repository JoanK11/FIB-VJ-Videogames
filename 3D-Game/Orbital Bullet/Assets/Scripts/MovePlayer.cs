using System.Collections;
using System.Collections.Generic;
using UnityEngine;



public class MovePlayer : MonoBehaviour {
    // Public fields should use PascalCase
    private float rotationSpeed, jumpSpeed, gravity;
    public GameObject prefab;

    // Private fields should use camelCase
    private Vector3 startDirection;
    private float speedY;
    private bool isFirst;
    private float changingLevelTime;
    bool ringChanging;

    public enum PlayerState { Normal, ChangingLevel, ChangingRing, Invincible };
    PlayerState State;

    // Start is called before the first frame update
    void Start() {
        // Store starting direction of the player with respect to the axis of the level
        startDirection = transform.position - transform.parent.position;
        startDirection.y = 0.0f;
        startDirection.Normalize();
        isFirst = true;
        speedY = 0;
        changingLevelTime = 0;
        ringChanging = false;

        State = PlayerState.Normal;

        rotationSpeed = 80;
        jumpSpeed = 8.25f;
        gravity = 25;
    }

    // Update is called once per frame
    void FixedUpdate() {
        CharacterController charControl = GetComponent<CharacterController>();

        bool canMove = true;
        if (State == PlayerState.ChangingLevel) {
            changingLevelTime += Time.deltaTime;
            if (charControl.isGrounded && changingLevelTime > 0.5f) {
                State = PlayerState.Normal;
                changingLevelTime = 0;
            }
            else canMove = false;
        }

        if (ringChanging) {
            canMove = false;
        }

        Vector3 position;

        float angle;
        Vector3 direction, target;

        position = transform.position;
        angle = rotationSpeed * Time.deltaTime;
        direction = position - transform.parent.position;

        // Left-right movement
        if ((Input.GetKey(KeyCode.A) || Input.GetKey(KeyCode.D)) && canMove) {

            if (Input.GetKey(KeyCode.A)) {
                target = transform.parent.position + Quaternion.AngleAxis(angle, Vector3.up) * direction;
                if (charControl.Move(target - position) != CollisionFlags.None) {
                    transform.position = position;
                    Physics.SyncTransforms();
                }
            }
            if (Input.GetKey(KeyCode.D)) {
                target = transform.parent.position + Quaternion.AngleAxis(-angle, Vector3.up) * direction;
                if (charControl.Move(target - position) != CollisionFlags.None) {
                    transform.position = position;
                    Physics.SyncTransforms();
                }
            }
        }

        //making dynamically bullets
        if (Input.GetKey(KeyCode.K) && isFirst) {
            isFirst = false;
            Vector3 bulletPos = transform.parent.position + Quaternion.AngleAxis(-angle - 10.0f, Vector3.up) * direction;
            Instantiate(prefab, bulletPos, Quaternion.identity, transform.parent);
        }
        // Correct orientation of player
        // Compute current direction
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

        // Apply up-down movement
        position = transform.position;
        if (!ringChanging && charControl.Move(speedY * Time.deltaTime * Vector3.up) != CollisionFlags.None) {
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

    public void JumpNextLevel() {
        CharacterController charControl = GetComponent<CharacterController>();

        if (charControl.isGrounded) {
            speedY = 20.0f;
            GameObject world = GameObject.Find("World");
            world.GetComponent<World>().NextLevel();
            State = PlayerState.ChangingLevel;
            Debug.Log(name + ": Player jumped to the next level.");
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
        ringChanging = false;
    }

    public void ChangeRing(Vector3 targetPosition) {
        ringChanging = true;
        StartCoroutine(MovePlayerToPosition(transform.position, targetPosition));
    }
    
}