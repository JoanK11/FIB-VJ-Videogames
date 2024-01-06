using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class HealthBarText : MonoBehaviour {
    public TMP_Text healthText;
    public Slider slider;

    float currentHealth;
    const float duration = 1f;

    void Start() {
        healthText.text = slider.value.ToString();
        currentHealth = slider.value;
    }

    void Update() {
        if (slider.value + 1 < currentHealth) {
            StartCoroutine(HighlightText(Color.red));
        }
        else if (slider.value - 1 > currentHealth) {
            StartCoroutine(HighlightText(Color.green));
        }

        healthText.text = slider.value.ToString();
        currentHealth = slider.value;
    }

    IEnumerator HighlightText(Color color) {
        healthText.color = color;
        yield return new WaitForSeconds(duration);
        healthText.color = Color.white;
    }
}
