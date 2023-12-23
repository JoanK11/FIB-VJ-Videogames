using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class EnemyStationary : EnemyBase {
    // Start is called before the first frame update
    public GameObject bala;
    Quaternion original;
    public float secondsInIdle;
    public float secondsBetweenBullets;
    float currentTime;
    public int numberOfBullets;
    float direction;
    int currentNumberOfBullets;
    enum EnemyState {
        IDLE,
        ATTACKING
    }
    EnemyState currentState;
    void Start() {
        base.init();
        original = transform.rotation;
        currentState = EnemyState.IDLE;
        currentTime = 0.0f;
        currentNumberOfBullets = 0;
    }

    // Update is called once per frame
    void FixedUpdate() {
        if (currentState == EnemyState.IDLE) {
            currentTime += Time.deltaTime;

            if (currentTime > secondsInIdle) {
                System.Random rand = new System.Random();
                currentTime = 0;
                int decision = rand.Next() % 100;
                Debug.Log("Decision " + decision);
                if (decision < 75) {
                    currentState = EnemyState.ATTACKING;

                    decision = rand.Next() % 100;
                    Debug.Log("Decision para orientacion " + decision);
                    direction = 1.0f;
                    if (decision < 42) {
                        direction = -1.0f;
                    }
                    Debug.Log("Orientacion" + direction);
                    currentNumberOfBullets = 0;
                }
            }
        }
        else if (currentState == EnemyState.ATTACKING) {
            if (currentTime == 0.0f) {
                makeBala();
                ++currentNumberOfBullets;
            }
            currentTime += Time.deltaTime;
            if (currentTime > secondsBetweenBullets) currentTime = 0.0f;
            if (currentNumberOfBullets >= numberOfBullets) currentState = EnemyState.IDLE;
        }
    }

    private void makeBala() {
        Vector3 orientation = transform.position - transform.parent.position;
        orientation.y = 0.0f;
        Vector3 bulletPos = transform.parent.position + Quaternion.AngleAxis(direction * 15.0f, Vector3.up) * orientation;
        bulletPos.y = transform.position.y;

        GameObject newObject = Instantiate(bala, bulletPos, transform.rotation, transform.parent);

        newObject.GetComponent<MoveBala>().setOrientation(-direction);
    }
}