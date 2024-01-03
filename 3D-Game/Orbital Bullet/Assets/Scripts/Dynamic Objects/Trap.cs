using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI; // Required for working with UI

public class Trap : MonoBehaviour {
    float damageInterval = 0.5f;
    int damageAmount = 10;
    float timer;

    /* -- UI -- */
    public Image screenFlash;
    float flashDuration = 0.5f;

    void OnTriggerEnter(Collider other) {
        if (other.CompareTag("Player")) {
            MovePlayer player = other.GetComponent<MovePlayer>();
            player.TakeDamage(damageAmount);
            StartCoroutine(FlashScreen());
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
                StartCoroutine(FlashScreen());
            }
        }
    }

    void OnTriggerExit(Collider other) {
        if (other.CompareTag("Player")) {
            timer = 0;
        }
    }

    IEnumerator FlashScreen() {
        screenFlash.color = new Color(screenFlash.color.r, screenFlash.color.g, screenFlash.color.b, 0.5f);
        float elapsed = 0;

        while (elapsed < flashDuration) {
            elapsed += Time.deltaTime;
            float alpha = Mathf.Lerp(0.5f, 0, elapsed / flashDuration);
            screenFlash.color = new Color(screenFlash.color.r, screenFlash.color.g, screenFlash.color.b, alpha);
            yield return null;
        }

        // Ensure the image is fully transparent again
        screenFlash.color = new Color(screenFlash.color.r, screenFlash.color.g, screenFlash.color.b, 0);
    }
}
