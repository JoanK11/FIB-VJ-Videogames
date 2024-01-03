using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MenuToogle : MonoBehaviour {
    public GameObject mainMenu;
    public MainMenu canvas;

    void Update() {
        if (Input.GetKeyDown(KeyCode.Escape) && this.gameObject.activeSelf) {
            canvas.PlayCancelSound();
            this.gameObject.SetActive(false);
            mainMenu.SetActive(true);
        }
    }
}
