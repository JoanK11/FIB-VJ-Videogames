using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BossLevel : MonoBehaviour {
    float rotationSpeed = 5.0f;
    bool exitedTrigger;
    public BoxCollider boxCollider;

    void Start() {
        exitedTrigger = false;
    }

    void Update() {
        if (exitedTrigger) {
            transform.Rotate(0, rotationSpeed * Time.deltaTime, 0);
        }
    }

    void OnTriggerExit(Collider other) {
        if (other.gameObject.CompareTag("Player")) {
            exitedTrigger = true;
            other.GetComponent<MovePlayer>().ArrivedNextLevel();
            boxCollider.isTrigger = false;
        }
    }
}
