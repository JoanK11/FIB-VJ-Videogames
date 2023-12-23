using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class EnemyHealthBar : MonoBehaviour {
    public Slider slider;
    public Camera camera;
    public Transform target;

    public void updateHealthBar(float currentHealth, float maxHealth) {
        slider.value = currentHealth / maxHealth;
    }
    void Update() {
        transform.rotation = camera.transform.rotation;
        transform.position = target.position + new Vector3(0, 1.0f, 0);
    }
}