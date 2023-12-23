using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Platform_Script : MonoBehaviour {
    private Collider objectCollider;
    public bool enteredTrigger;
    public bool exitedTrigger;

    void Start() {
        objectCollider = GetComponent<Collider>();
        if (objectCollider != null) {
            objectCollider.isTrigger = true;
        }
        else {
            Debug.LogError(name + ": Collider component not found on the object.");
        }

        enteredTrigger = false;
        exitedTrigger = false;
    }

    void OnTriggerEnter(Collider other) {
        if (other.gameObject.CompareTag("Player")) {
            Debug.Log(name + ": Player entered platform.");
            enteredTrigger = true;
        }
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
