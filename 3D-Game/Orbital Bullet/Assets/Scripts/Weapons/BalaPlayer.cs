using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using UnityEditor;

public class BalaPlayer : BalaBase {
    GameObject prefab;
    private float Distance;
    const float maxDistance = 50f;

    public void init() {
        rotationSpeed = -70.0f;
        damage = 50.0f;
        base.initBala();

        Distance = 0.0f;
        prefab = Resources.Load<GameObject>("SmallExplosion");
        Center = GameObject.Find("Player").GetComponent<MovePlayer>().GetCenter();
    }

    void Update() {
        // Destroy the bullet if it has already exploded and finished making the sound
        if (exploded && !audioSource.isPlaying) {
            Destroy(gameObject);
            return;
        }
        if (exploded) return;
        Move();
        Distance += Math.Abs(rotationSpeed * Time.deltaTime);

        if (Distance >= maxDistance) {
            audioSource.volume = distanceVolume;
            PlayExplosionSound();
            GameObject explosion = Instantiate(prefab, transform.position, Quaternion.identity);
            Destroy(explosion, 1.0f);
        }
    }

    protected void OnTriggerEnter(Collider other) {
        if (exploded) return;

        if (other.gameObject.tag == "Enemy") {
            EnemyBase enemy = other.gameObject.GetComponent<EnemyBase>();
            enemy.takeDamage(GetDamage());
        }
        else {
            audioSource.volume = distanceVolume;
        }

        PlayExplosionSound();
        GetComponent<Renderer>().enabled = false;
        GameObject explosion = Instantiate(prefab, transform.position, Quaternion.identity);
        Destroy(explosion, 1.0f);
    }
    
}