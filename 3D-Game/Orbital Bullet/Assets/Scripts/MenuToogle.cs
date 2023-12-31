using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MenuToogle : MonoBehaviour {
    public GameObject mainMenu;

    void Start() {
        //this.gameObject.SetActive(false);

        if (mainMenu == null) {
            Debug.LogError("'MainMenu' GameObject does not exist in " + name + ".");
        }
    }

    void Update() {
        if (Input.GetKeyDown(KeyCode.Escape) && this.gameObject.activeSelf) {
            this.gameObject.SetActive(false);
            mainMenu.SetActive(true);
        }
    }
}
