using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI; // Required for working with UI

public class RedDamage : MonoBehaviour {
    Image screenFlash;
    const float flashDuration = 0.5f;

    void Start() {
        screenFlash = GetComponent<Image>();
    }

    public IEnumerator FlashScreen() {
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
