using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class CurrentAmmo : MonoBehaviour {
    TMP_Text ammoText;
    int currentAmmo;
    const float duration = 1f;

    void Start() {
        ammoText = gameObject.GetComponentInChildren<TMP_Text>();
        currentAmmo = 10;
    }

    public void SetAmmo(int ammo) {
        if (ammo > currentAmmo) {
            StartCoroutine(HighlightText());
        }
        ammoText.text = ammo.ToString();
        currentAmmo = ammo;
    }

    IEnumerator HighlightText() {
        ammoText.color = Color.green;
        yield return new WaitForSeconds(duration);
        ammoText.color = Color.white;
    }
}