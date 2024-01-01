using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Trap : MonoBehaviour {
    float damageInterval = 0.5f;
    int damageAmount = 10;
    float timer;

    void OnTriggerEnter(Collider other) {
        if (other.CompareTag("Player")) {
            MovePlayer player = other.GetComponent<MovePlayer>();
            player.TakeDamage(damageAmount);
        }
    }

    void OnTriggerStay(Collider other) {
        if (other.CompareTag("Player")) {
            timer += Time.deltaTime;

            // If the timer exceeds the damage interval, damage the player
            if (timer >= damageInterval) {
                timer = 0;

                MovePlayer player = other.GetComponent<MovePlayer>();
                player.TakeDamage(damageAmount);
            }
        }
    }

    void OnTriggerExit(Collider other) {
        if (other.CompareTag("Player")) {
            timer = 0;
        }
    }
}
