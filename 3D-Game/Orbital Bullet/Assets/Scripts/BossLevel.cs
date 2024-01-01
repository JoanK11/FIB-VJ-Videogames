using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BossLevel : MonoBehaviour {
    float rotationSpeed = 25.0f;
    public bool exitedTrigger;

    void Start() {
        exitedTrigger = false;
    }

    void FixedUpdate() {
        if (exitedTrigger) {
            transform.Rotate(0, rotationSpeed * Time.deltaTime, 0);
        }
    }

    void OnTriggerExit(Collider other) {
        if (other.gameObject.CompareTag("Player")) {
            exitedTrigger = true;
            other.GetComponent<MovePlayer>().ArrivedNextLevel();
            GetComponent<BoxCollider>().isTrigger = false;
        }
    }
}
