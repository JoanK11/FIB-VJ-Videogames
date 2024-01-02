using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro; // TextMesh Pro namespace
using UnityEngine.UI; // Image namespace

public class Jump : MonoBehaviour {
    public GameObject player;
    private bool isPlayerOnTrigger;

    /* -- UI -- */
    public GameObject UIButton;
    TextMeshProUGUI UItext;
    Image UIimage;
    Sprite keySprite;

    void Start() {
        isPlayerOnTrigger = false;

        player = GameObject.Find("Player");
        if (player == null) {
            Debug.LogError(name + ": Player object not found. Make sure your player is named 'Player'.");
        }

        if (UIButton == null) {
            Debug.LogError(name + ": UIButton not found.");
        } else {
            UItext = UIButton.GetComponentInChildren<TextMeshProUGUI>();
            UIimage = UIButton.GetComponentInChildren<Image>();
            keySprite = Resources.Load<Sprite>("E-Key");
        }
    }

    void Update() {
        if (isPlayerOnTrigger && Input.GetKeyUp(KeyCode.E)) {
            player.GetComponent<MovePlayer>().JumpNextLevel();
            Debug.Log(name + ": Player tried to jump to the next level.");
        }
    }

    void OnTriggerEnter(Collider other) {
        if (other.gameObject == player) {
            isPlayerOnTrigger = true;
            ShowUI(true);
            Debug.Log(name + ": Player entered Trigger.");
        }
    }

    void OnTriggerExit(Collider other) {
        if (other.gameObject == player) {
            isPlayerOnTrigger = false;
            ShowUI(false);
            Debug.Log(name + ": Player exited Trigger.");
        }
    }

    void ShowUI(bool show) {
        if (show) {
            UItext.text = "Jump to the next sector";
            UIimage.sprite = keySprite;
            UIButton.SetActive(true);
        } else {
            UIButton.SetActive(false);
        }
    }
}
