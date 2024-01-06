using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class EnemyBase : MonoBehaviour {
    public float maxHealth;
    float health;

    public float maxShield;
    float shield;
    EnemyHealthBar healthBar;
    EnemyHealthBar shieldBar;

    /* -- Sound Effects -- */
    protected AudioSource audioSource;
    public AudioClip dieSound1, dieSound2;
    protected bool playedSound;
    Camera c;
    public void init() {
        EnemyHealthBar[] bars = GetComponentsInChildren<EnemyHealthBar>();
        healthBar = bars[0];
        shieldBar = bars[1];
        shield = maxShield;
        health = maxHealth;
        c = GameObject.FindWithTag("MainCamera").GetComponent<Camera>();
        /* -- Sound Effects -- */
        audioSource = GetComponent<AudioSource>();
        playedSound = false;
    }
    protected void lookCamera() {
        transform.rotation = c.transform.rotation;
    }

    void FixedUpdate() {
        // Destroy the enemy if it has died and finished making the sound
        if (playedSound && !audioSource.isPlaying) {
            Destroy(gameObject);
            if (gameObject.tag == "Enemy") {
                transform.parent.gameObject.GetComponent<EnemyManager>().EnemyDefeated();
            }
        }
    }

    public void takeDamage(float damageAmount) {
        if (shield > 0) {
            shield -= damageAmount;

            if (shield <= 0) {
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
            if (health <= 0) {
                healthBar.gameObject.SetActive(false);
                PlayDieSound();
            }
        }
    }

    protected void PlayDieSound() {
        if (playedSound) return;

        int random = UnityEngine.Random.Range(0, 2); // [min, max)
        if (random == 0) audioSource.clip = dieSound1;
        else audioSource.clip = dieSound2;

        if (audioSource.clip == null) {
            Debug.LogError("audioSource.clip is null in EnemyBase");
        }
        audioSource.Play();

        playedSound = true;
    }
}