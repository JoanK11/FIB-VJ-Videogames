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
    Camera c;
    public void init() {
        EnemyHealthBar[] bars = GetComponentsInChildren<EnemyHealthBar>();
        healthBar = bars[0];
        shieldBar = bars[1];
        shield = maxShield;
        health = maxHealth;
        c = GameObject.FindWithTag("MainCamera").GetComponent<Camera>();
    }
    protected void lookCamera() {
        transform.rotation = c.transform.rotation;
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

                Destroy(gameObject);
                if (gameObject.tag == "Enemy") transform.parent.gameObject.GetComponent<EnemyManager>().EnemyDefeated();
            }
        }
    }
}