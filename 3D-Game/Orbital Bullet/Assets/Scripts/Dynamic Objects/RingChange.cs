using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro; // TextMesh Pro namespace
using UnityEngine.UI; // Image namespace

public class RingChange : MonoBehaviour {
    public GameObject target;
    GameObject player;
    bool isPlayerOnTrigger;
    const float duration = 0.6f;

    /* -- UI -- */
    public GameObject UIButton;
    TextMeshProUGUI UItext;
    Image UIimage;
    Sprite keySprite;

    void Start() {
        InitializeGameObjects();
        isPlayerOnTrigger = false;
    }

    void InitializeGameObjects() {
        player = GameObject.Find("Player");

        UItext = UIButton.GetComponentInChildren<TextMeshProUGUI>();
        UIimage = UIButton.GetComponentInChildren<Image>();
        keySprite = Resources.Load<Sprite>("E-Key");
    }

    void Update() {
        if (isPlayerOnTrigger && Input.GetKeyUp(KeyCode.E)) {
            Vector3 targetPosition = target.transform.position + Vector3.up;
            Debug.Log(name + ": Teleporting Player to " + targetPosition);
            player.GetComponent<MovePlayer>().ChangeRing(targetPosition, duration);
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
