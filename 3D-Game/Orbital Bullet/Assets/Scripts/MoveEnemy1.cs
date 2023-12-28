using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveEnemy1 : EnemyBase
{
    // Start is called before the first frame update
    public float rotationSpeed, jumpSpeed, gravity;


    Vector3 startDirection;
    float speedY;


    Quaternion originalrotation;
    bool isRight;

  
    CharacterController charControl;
    Transform reference;
    void Start()
    {
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
    }
    // Update is called once per frame
    void Update() {
        Vector3 position;

        float angle;
        Vector3 direction, target;

        position = transform.position;
        angle = rotationSpeed * Time.deltaTime;
        direction = position - reference.position;

        // Left-right movement

        target = reference.position + Quaternion.AngleAxis(angle, Vector3.up) * direction;
        if (charControl.Move(target - position) != CollisionFlags.None)
        {
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
        if (charControl.Move(speedY * Time.deltaTime * Vector3.up) != CollisionFlags.None)
        {
            transform.position = position;
            Physics.SyncTransforms();
        }
        if (charControl.isGrounded)
        {
            if (speedY < 0.0f)
                speedY = 0.0f;
           
        }
        else
            speedY -= gravity * Time.deltaTime;
        
        
    }
    
    void OnControllerColliderHit(ControllerColliderHit hit) {
        if(hit.gameObject.tag != "Floor")
        Debug.Log("he hiteado a " + hit.gameObject.name);
    }

}
