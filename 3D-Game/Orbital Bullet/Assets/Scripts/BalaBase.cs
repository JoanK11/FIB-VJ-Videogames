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

    protected void initBala() {
        startDirection = transform.position - transform.parent.position;
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
        direction = position - transform.parent.position;

        target = transform.parent.position + Quaternion.AngleAxis(angle, Vector3.up) * direction;

        transform.position = target;
        Physics.SyncTransforms();


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
        transform.rotation = orientation * rotacionInicial;
    }
    public float GetDamage() { 
        return damage;
    }
    public void setOrientation(float newDirection)
    {
        rotationSpeed = newDirection * Math.Abs(rotationSpeed);
    }
}
