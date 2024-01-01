using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro; // TextMesh Pro namespace
using UnityEngine.UI; // Image namespace

public class Chest : MonoBehaviour {
    /* -- Chest -- */
    public GameObject chest;
    GameObject player;
    bool isPlayerOnTrigger;
    bool isChestOpened;

    /* -- UI -- */
    public GameObject UIButton;
    TextMeshProUGUI UItext;
    Image UIimage;
    Sprite keySprite;

    /* -- Animation -- */
    Animator animator;

    /* -- Chest -- */
    PlayerAudio playerAudio;

    void Start () {
        isPlayerOnTrigger = false;
        isChestOpened = false;
        animator = chest.GetComponent<Animator>();
        InitializeGameObjects();
    }

    void InitializeGameObjects() {
        player = GameObject.Find("Player");
        if (player == null) {
            Debug.LogError(name + ": Player object not found. Make sure your player is named 'Player'.");
        }
        playerAudio = player.GetComponent<PlayerAudio>();

        if (UIButton == null) {
            Debug.LogError(name + ": UIButton not found.");
        }
        else {
            UItext = UIButton.GetComponentInChildren<TextMeshProUGUI>();
            UIimage = UIButton.GetComponentInChildren<Image>();
            keySprite = Resources.Load<Sprite>("E-Key");
        }
    }

    void Update() {
        if (isPlayerOnTrigger && Input.GetKeyUp(KeyCode.E) && !isChestOpened) {
            isChestOpened = true;
            animator.SetTrigger("move");
            playerAudio.PlayChestSound();
            Debug.Log(name + ": Player opened chest.");
        }
    }

    void OnTriggerEnter(Collider other) {
        if (other.gameObject == player && !isChestOpened) {
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
        if (show && !isChestOpened) {
            UItext.text = "Open Chest";
            UIimage.sprite = keySprite;
            UIButton.SetActive(true);
        }
        else {
            UIButton.SetActive(false);
        }
    }
}
