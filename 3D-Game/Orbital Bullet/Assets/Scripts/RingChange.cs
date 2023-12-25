using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro; // TextMesh Pro namespace
using UnityEngine.UI; // Image namespace

public class RingChange : MonoBehaviour {
    public GameObject target;
    GameObject player;
    bool isPlayerOnTrigger;

    /* -- UI -- */
    public GameObject UIButton;
    TextMeshProUGUI UItext;
    Image UIimage;
    Sprite keySprite;


    void Start() {
        // Initialize GameObjects
        InitializeGameObjects();

        isPlayerOnTrigger = false;
    }

    void Update() {
        if (isPlayerOnTrigger && Input.GetKeyUp(KeyCode.E)) {
            Vector3 targetPosition = target.transform.position + new Vector3(0, 1.0f, 0);
            player.GetComponent<MovePlayer>().ChangeRing(targetPosition);
        }
    }

    void InitializeGameObjects() {
        player = GameObject.Find("Player");
        if (player == null) {
            Debug.LogError(name + ": Player object not found. Make sure your player is named 'Player'.");
        }

        if (target == null) {
            Debug.LogError(name + ": Ring Change has no target.");
        }

        if (UIButton == null) {
            Debug.LogError(name + ": UIButton not found.");
        } else {
            UItext = UIButton.GetComponentInChildren<TextMeshProUGUI>();
            UIimage = UIButton.GetComponentInChildren<Image>();
            keySprite = Resources.Load<Sprite>("E-Key");
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
            UItext.text = "Switch Ring";
            UIimage.sprite = keySprite;
            UIButton.SetActive(true);
        } else {
            UIButton.SetActive(false);
        }
    }
}
