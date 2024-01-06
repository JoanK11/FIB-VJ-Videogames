using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveEnemy1 : EnemyBase {
    public float rotationSpeed, jumpSpeed, gravity;

    Vector3 startDirection;
    float speedY;

    Quaternion originalrotation;
    bool isRight;
  
    CharacterController charControl;
    Transform reference;
    const float timeAttack= 3.0f;
    float time;
    bool canAttack;
    const int attackDamage = 10;
    void Start() {
        reference = GameObject.Find("World").transform;

        // Store starting direction of the player with respect to the axis of the level
        startDirection = transform.position - reference.position;
        startDirection.y = 0.0f;
        startDirection.Normalize();
        speedY = 0;

        originalrotation = transform.rotation;
        isRight = true;

        charControl = GetComponent<CharacterController>();
        charControl.detectCollisions = true;

        base.init();
        time = 0;
        canAttack = true;
    }

    void FixedUpdate() {
        // Destroy the enemy if it has died and finished making the sound
        if (playedSound && !audioSource.isPlaying) {
            Destroy(gameObject);
            if (gameObject.tag == "Enemy") {
                transform.parent.gameObject.GetComponent<EnemyManager>().EnemyDefeated();
            }
            return;
        }
        if (playedSound)
        {
            GetComponent<Collider>().enabled = false;
            charControl.enabled = false;
            return;

        }
        if (!canAttack) {
            time += Time.deltaTime;
            if (time >= timeAttack) {
                canAttack = true;
            }
        }
        Vector3 position = transform.position;

        float angle = rotationSpeed * Time.deltaTime;
        Vector3 direction = position - reference.position;
        Vector3 target;

        // Left-right movement
        target = reference.position + Quaternion.AngleAxis(angle, Vector3.up) * direction;
        if (charControl.Move(target - position) != CollisionFlags.None) {
            transform.position = position;
            rotationSpeed *= -1;
            isRight = !isRight;
            
        }

        // Correct orientation of player
        // Compute current direction
        Vector3 currentDirection = transform.position - reference.position;
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
        transform.rotation = orientation* originalrotation;
        if (!isRight) transform.rotation *= Quaternion.Euler(0, 180, 0);


        // Apply up-down movement
        position = transform.position;
        if (charControl.Move(speedY * Time.deltaTime * Vector3.up) != CollisionFlags.None) {
            transform.position = position;
            Physics.SyncTransforms();
        }
        if (charControl.isGrounded) {
            if (speedY < 0.0f)
                speedY = 0.0f;
           
        }
        else
            speedY -= gravity * Time.deltaTime;
        lookCamera();

    }
    
    void OnControllerColliderHit(ControllerColliderHit hit) {
        if (hit.gameObject.tag == "Floor") return;
        //Debug.Log("he hiteado a " + hit.gameObject.name);
        if (canAttack && hit.gameObject.tag == "Player") {
            canAttack = false;
            time = 0;
            hit.gameObject.GetComponent<MovePlayer>().TakeDamage(attackDamage);
        }
    }
}
