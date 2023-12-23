using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Jump : MonoBehaviour {
    public GameObject player;
    private bool isPlayerOnTrigger;

    void Start() {
        isPlayerOnTrigger = false;

        player = GameObject.Find("Player");
        if (player == null) {
            Debug.LogError(name + ": Player object not found. Make sure your player is named 'Player'.");
        }
    }

    void Update() {
        if (isPlayerOnTrigger && Input.GetKeyUp(KeyCode.B)) {
            player.GetComponent<MovePlayer>().JumpNextLevel();
            Debug.Log(name + ": Player tried to jump to the next level.");
        }
    }

    void OnTriggerEnter(Collider other) {
        if (other.gameObject == player) {
            isPlayerOnTrigger = true;
            Debug.Log(name + ": Player entered Trigger.");
        }
    }

    void OnTriggerExit(Collider other) {
        if (other.gameObject == player) {
            isPlayerOnTrigger = false;
            Debug.Log(name + ": Player exited Trigger.");
        }
    }
}
