using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro; // TextMesh Pro namespace
using UnityEngine.UI; // Image namespace

public class ChestHealth : MonoBehaviour {
    /* -- Movement -- */
    float startY;
    const float amplitude = 0.2f; // The height of the oscillation
    const float frequency = 1f; // The speed of the oscillation

    /* -- Player -- */
    GameObject player;
    bool isPlayerOnTrigger;
    const int health = 50;

    /* -- UI Button -- */
    public GameObject UIButton;
    TextMeshProUGUI UItext;
    Image UIimage;
    Sprite keySprite;

    /* -- Sound Effects -- */
    AudioSource audioSource;
    bool pickedUp;

    void Start() {
        /* -- Movement -- */
        startY = transform.position.y;

        /* -- Player -- */
        player = GameObject.Find("Player");
        isPlayerOnTrigger = false;

        /* -- UI -- */
        UItext = UIButton.GetComponentInChildren<TextMeshProUGUI>();
        UIimage = UIButton.GetComponentInChildren<Image>();
        keySprite = Resources.Load<Sprite>("E-Key");

        /* -- Sound Effects -- */
        audioSource = GetComponent<AudioSource>();
        pickedUp = false;
    }

    void Update() {
        // Delete Object
        if (pickedUp && !audioSource.isPlaying) {
            Destroy(gameObject);
        }

        // UI
        if (isPlayerOnTrigger && Input.GetKeyUp(KeyCode.E) && !pickedUp) {
            pickedUp = true;
            ShowUI(false);

            // Disable MeshRenderer
            MeshRenderer mr = gameObject.GetComponent<MeshRenderer>();
            mr.enabled = false;

            // Play audio
            if (audioSource.clip == null) Debug.LogError("audiosource.clip is null in ChestHealth");
            audioSource.Play();

            // Increase Player Health
            player.GetComponent<MovePlayer>().IncreaseHealth(health);
        }

        if (!pickedUp) {
            // Oscillate up and down over time
            transform.position = new Vector3(transform.position.x,
                startY + Mathf.Sin(Time.time * Mathf.PI * frequency) * amplitude,
                transform.position.z);
        }
    }

    void OnTriggerEnter(Collider other) {
        if (other.gameObject == player && !pickedUp) {
            isPlayerOnTrigger = true;
            ShowUI(true);
        }
    }

    void OnTriggerExit(Collider other) {
        if (other.gameObject == player) {
            isPlayerOnTrigger = false;
            ShowUI(false);
        }
    }

    void ShowUI(bool show) {
        if (show && !pickedUp) {
            UItext.text = "Pick Up " + health + " Health";
            UItext.color = Color.white;
            UIimage.sprite = keySprite;
            UIButton.SetActive(true);
        }
        else {
            UIButton.SetActive(false);
        }
    }
}
