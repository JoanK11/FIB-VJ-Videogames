using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Jump : MonoBehaviour {
    public GameObject player;
    private bool isPlayerOnTrigger;
    private bool objectTriggered;

    void Start() {
        isPlayerOnTrigger = false;
        objectTriggered = false;

        // GameObjects initialization
        player = GameObject.Find("Player");
        if (player == null) {
            Debug.LogError("Player object not found. Make sure your player is named 'Player'");
        }
    }

    void Update() {
        if (isPlayerOnTrigger && !objectTriggered && Input.GetKeyUp(KeyCode.B)) {
            player.GetComponent<MovePlayer>().JumpNextLevel();
            Debug.Log("Player tried to jump to the next level.");
        }
    }   

    void OnTriggerEnter(Collider other) {
        // Check if the player entered the trigger
        if (other.gameObject == player) {
            isPlayerOnTrigger = true;
            Debug.Log("Player is on Trigger");
        }
    }

    void OnTriggerExit(Collider other) {
        // Check if the player exited the trigger
        if (other.gameObject == player) {
            isPlayerOnTrigger = false;
            Debug.Log("Player exited Trigger");
        }
    }
}
