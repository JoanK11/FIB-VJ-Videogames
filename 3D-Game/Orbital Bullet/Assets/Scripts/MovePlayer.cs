using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MovePlayer : MonoBehaviour
{
    public float rotationSpeed, jumpSpeed, gravity;
    public float timeToRestartShoot;
    float restartTime;
    public GameObject prefab;
    Vector3 startDirection;
    float speedY;
    bool reloading;
    int num;
    // Start is called before the first frame update
    void Start()
    {
        // Store starting direction of the player with respect to the axis of the level
        startDirection = transform.position - transform.parent.position;
        startDirection.y = 0.0f;
        startDirection.Normalize();
        reloading= false;
        speedY = 0;
        num = 0;
    }
   
    // Update is called once per frame
    void FixedUpdate()
    {
        CharacterController charControl = GetComponent<CharacterController>();
        Vector3 position;

        float angle;
        Vector3 direction, target;

        position = transform.position;
        angle = rotationSpeed * Time.deltaTime;
        direction = position - transform.parent.position;

        // Left-right movement
        if (Input.GetKey(KeyCode.A) || Input.GetKey(KeyCode.D))
        {
            
            if (Input.GetKey(KeyCode.A))
            {
                target = transform.parent.position + Quaternion.AngleAxis(angle, Vector3.up) * direction;
                if (charControl.Move(target - position) != CollisionFlags.None)
                {
                    transform.position = position;
                    Physics.SyncTransforms();
                }
            }
            if (Input.GetKey(KeyCode.D))
            {
                target = transform.parent.position + Quaternion.AngleAxis(-angle, Vector3.up) * direction;
                if (charControl.Move(target - position) != CollisionFlags.None)
                {
                    transform.position = position;
                    Physics.SyncTransforms();
                }
            }
        }
        //making dynamically bullets
       
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

        if (Input.GetKey(KeyCode.K) && !reloading)
        {
            reloading = true;
            restartTime = 0.0f;
            Vector3 bulletPos = transform.parent.position + Quaternion.AngleAxis(-angle - 15.0f, Vector3.up) * direction;
            bulletPos.y = transform.position.y;
            Debug.Log(bulletPos);
            GameObject newObject = Instantiate(prefab, bulletPos, transform.rotation, transform.parent);
            newObject.name = "bala" + (++num);
        }
        if (reloading)
        {
            restartTime += Time.deltaTime;
            if (restartTime >= timeToRestartShoot)
            {
                reloading = false;
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
            if (speedY < 0.0f)
                speedY = 0.0f;
            if (Input.GetKey(KeyCode.W))
                speedY = jumpSpeed;
        }
        else
            speedY -= gravity * Time.deltaTime;
    }
}


