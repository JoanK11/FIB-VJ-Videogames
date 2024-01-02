using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class BalaBase : MonoBehaviour
{
    
    protected float rotationSpeed;

    protected float damage;

    protected Vector3 startDirection;

    protected Quaternion rotacionInicial;
    protected Vector3 Center;
    protected void initBala() {
  
        startDirection = transform.position -Center;
        startDirection.y = 0.0f;
        startDirection.Normalize();
        rotacionInicial = transform.rotation;
    }
    protected void Move() {
        //   Debug.Log("entro en el update");

        Vector3 position;
        float angle;
        Vector3 direction, target;

        position = transform.position;
        angle = rotationSpeed * Time.deltaTime;
        direction = position - Center;

        target = Center + Quaternion.AngleAxis(angle, Vector3.up) * direction;

        transform.position = target;
        Physics.SyncTransforms();


        // Correct orientation of player
        // Compute current direction

        Vector3 currentDirection = transform.position - Center;
        currentDirection.y = 0.0f;
        currentDirection.Normalize();
      
        // Change orientation of player accordingly
        Vector3 forward = Vector3.Cross(currentDirection, Vector3.up);
        transform.rotation=Quaternion.LookRotation(forward);
        
    }
    public float GetDamage() { 
        return damage;
    }
    public void setOrientation(float newDirection)
    {
        rotationSpeed = newDirection * Math.Abs(rotationSpeed);
    }
}
