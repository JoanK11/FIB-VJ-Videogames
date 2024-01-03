using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Enemy3 : EnemyBase
{
    // Start is called before the first frame update
    float jumpSpeed, gravity;


    Vector3 startDirection;
    float speedY;


    Quaternion originalrotation;


    CharacterController charControl;
    const float timeToJump = 4.0f;
    float time;
    Transform reference;

    bool canAttack;
    const float timeToRestartAttack = 6.0f;
    float timeAttack;

    bool isJumping;
    void Start()
    {
        reference = GameObject.Find("World").transform;
        // Store starting direction of the player with respect to the axis of the level
        startDirection = transform.position - reference.position;
        startDirection.y = 0.0f;
        startDirection.Normalize();
        speedY = 0;

        originalrotation = transform.rotation;


        charControl = GetComponent<CharacterController>();
        charControl.detectCollisions = true;

        base.init();
        time = 0;
        jumpSpeed = 16f;
        gravity = 25;
        canAttack = true;
        isJumping = false;

    }
    // Update is called once per frame
    void FixedUpdate()
    {
        Vector3 position;

        Vector3 direction;

        position = transform.position;
        direction = position - reference.position;

        // Left-right movement
        if (!canAttack) {

            timeAttack += Time.deltaTime;
            if (timeAttack >= timeToRestartAttack) canAttack = true;
        }

        if (!isJumping) {
            time += Time.deltaTime;
            Debug.Log(time);
            if (time >= timeToJump)
            {
                Debug.Log("He entrado enemigo3");
                time = 0;
                speedY = jumpSpeed;

            }

        }


        // Apply up-down movement
        position = transform.position;
        if (charControl.Move(speedY * Time.deltaTime * Vector3.up) != CollisionFlags.None)
        {
            transform.position = position;
            Physics.SyncTransforms();
        }
        if (charControl.isGrounded)
        {
            isJumping = false;
            
            if (speedY < 0.0f)
                speedY = 0.0f;

        }
        else
            speedY -= gravity * Time.deltaTime;

        lookCamera();
    }

    void OnControllerColliderHit(ControllerColliderHit hit)
    {
        if (hit.gameObject.tag != "Floor")
            Debug.Log("he hiteado a " + hit.gameObject.name);
        if (canAttack && hit.gameObject.tag == "Player") {
            hit.gameObject.GetComponent<MovePlayer>().TakeDamage(20);
            canAttack = false;
            timeAttack = 0;
        }
    }
}
