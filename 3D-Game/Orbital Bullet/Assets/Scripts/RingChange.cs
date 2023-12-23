using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RingChange : MonoBehaviour {
    private GameObject player;
    public GameObject target;
    private bool isPlayerOnTrigger;

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

    private void InitializeGameObjects() {
        player = GameObject.Find("Player");

        if (player == null) {
            Debug.LogError(name + ": Player object not found. Make sure your player is named 'Player'.");
        }

        if (target == null) {
            Debug.LogError(name + ": Ring Change has no target.");
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
