using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ShowCurrentWeapon : MonoBehaviour {
    GameObject[] images;
    GameObject[] weapons;

    void Start() {
        /* -- Weapons -- */
        weapons = new GameObject[2];
        weapons[0] = GameObject.Find("Bow1");
        weapons[1] = GameObject.Find("Bow2");

        /* -- Images -- */
        images = new GameObject[2];
        images[0] = GameObject.Find("BowLittle");
        images[1] = GameObject.Find("BowLarge");
    }

    void Update() {
        if (weapons[0].activeSelf) {
            images[0].gameObject.SetActive(true);
            images[1].gameObject.SetActive(false);
        }
        else {
            images[0].gameObject.SetActive(false);
            images[1].gameObject.SetActive(true);
        }
    }
}
