using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Platform_Script : MonoBehaviour {
    private Collider objectCollider;
    public bool exitedTrigger;

    void Start() {
        objectCollider = GetComponent<Collider>();
        if (objectCollider != null) {
            objectCollider.isTrigger = true;
        }
        else {
            Debug.LogError(name + ": Collider component not found on the object.");
        }

        exitedTrigger = false;
    }

    void OnTriggerExit(Collider other) {
        if (other.gameObject.CompareTag("Player")) {
            Debug.Log(name + ": Player exited platform.");
            exitedTrigger = true;
        }
    }

    public void ActivateCollider() {
        // Check if the collider exists
        if (objectCollider != null) {
            // Change the collider to act as a physical collider, not a trigger
            objectCollider.isTrigger = false;
        }
        else {
            Debug.LogError("Collider component not found on the object.");
        }
    }
}
