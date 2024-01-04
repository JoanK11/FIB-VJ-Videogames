using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class BalaBase : MonoBehaviour {
    protected float rotationSpeed;

    protected float damage;

    protected Vector3 startDirection;

    protected Vector3 Center;

    /* -- Bullet State -- */
    protected bool exploded;

    /* -- Sound Effects -- */
    protected AudioSource audioSource;
    protected const float distanceVolume = 0.1f;

    protected void initBala() {
        startDirection = transform.position - Center;
        startDirection.y = 0.0f;
        startDirection.Normalize();

        /* -- Bullet State -- */
        exploded = false;

        /* -- Sound Effects -- */
        audioSource = GetComponent<AudioSource>();
    }

    protected void Move() {
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
        transform.rotation = Quaternion.LookRotation(forward);
        if (rotationSpeed >= 0) transform.rotation *= Quaternion.Euler(0, 180, 0);
    }

    public float GetDamage() { 
        return damage;
    }

    public void setOrientation(float newDirection) {
        rotationSpeed = newDirection * Math.Abs(rotationSpeed);
    }

    protected void PlayExplosionSound() {
        if (exploded) return;
        Debug.Log("---------------------------- volume is " + audioSource.volume);

        if (audioSource == null) {
            Debug.LogError("audioSource is null in BalaBase");
        }
        if (audioSource.clip == null) {
            Debug.LogError("audioSource.clip is null in BalaBase");
        }

        audioSource.Play();
        exploded = true;
    }
}
