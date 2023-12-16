using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class MoveEnemy1 : MonoBehaviour
{
    // Start is called before the first frame update
    public float rotationSpeed, jumpSpeed, gravity;

    public float maxHealth;
    float health;

    public float maxShield;
    float shield;
    Vector3 startDirection;
    float speedY;


    Quaternion originalrotation;
    bool isRight;

  
    CharacterController charControl;
    EnemyHealthBar healthBar;
    EnemyHealthBar shieldBar;
    void Start()
    {
        // Store starting direction of the player with respect to the axis of the level
        startDirection = transform.position - transform.parent.position;
        startDirection.y = 0.0f;
        startDirection.Normalize();
        speedY = 0;

        originalrotation = transform.rotation;
        isRight = true;


        charControl = GetComponent<CharacterController>();
        charControl.detectCollisions = true;
        EnemyHealthBar[] bars = GetComponentsInChildren<EnemyHealthBar>();
        Debug.Log(bars.Length);
        healthBar =bars[0];
        shieldBar = bars[1];
        shield = maxShield;
        health = maxHealth;
    }
    // Update is called once per frame
    void Update() {
        Vector3 position;

        float angle;
        Vector3 direction, target;

        position = transform.position;
        angle = rotationSpeed * Time.deltaTime;
        direction = position - transform.parent.position;

        // Left-right movement

        target = transform.parent.position + Quaternion.AngleAxis(angle, Vector3.up) * direction;
        if (charControl.Move(target - position) != CollisionFlags.None)
        {
            transform.position = position;
            rotationSpeed *= -1;
            isRight = !isRight;
            
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

    public void takeDamage(float damageAmount) {
        if (shield > 0)
        {
            shield -= damageAmount;
            if (shield <= 0)
            {
                float rest = Math.Abs(shield);
                health -= rest;
                healthBar.updateHealthBar(health, maxHealth);
                shieldBar.gameObject.SetActive(false);
            }
            else shieldBar.updateHealthBar(shield, maxShield);
        }
        else {
            health -= damageAmount;
            healthBar.updateHealthBar(health, maxHealth);
            if (health <= 0)
            {   
                gameObject.SetActive(false);
                Destroy(this);

            }
        }
    }
}
