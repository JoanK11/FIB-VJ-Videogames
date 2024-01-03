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

    Transform reference;
    Quaternion rotacionInicial;
    void Start() {
        base.init();
        original = transform.rotation;
        currentState = EnemyState.IDLE;
        currentTime = 0.0f;
        currentNumberOfBullets = 0;
        reference = GameObject.Find("World").transform ;
        rotacionInicial = transform.rotation ;
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

        if (currentState == EnemyState.IDLE) {
            currentTime += Time.deltaTime;
            transform.rotation = rotacionInicial ;
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
                    if (decision < 42)
                    {
                        direction = -1.0f;
                        transform.rotation *= Quaternion.Euler(0,180,0);
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
        Vector3 orientation = transform.position - reference.position;
        orientation.y = 0.0f;
        Vector3 bulletPos = reference.position + Quaternion.AngleAxis(-direction * 5.0f, Vector3.up) * orientation;
        bulletPos.y = transform.position.y;

        GameObject newObject = Instantiate(bala, bulletPos, transform.rotation, reference);
        BalaEnemy bEnemy = newObject.AddComponent<BalaEnemy>();
        bEnemy.init();
        bEnemy.setOrientation(-direction);
    }
}