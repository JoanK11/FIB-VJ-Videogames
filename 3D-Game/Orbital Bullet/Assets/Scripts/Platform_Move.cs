using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Platform_Move : MonoBehaviour {
    public GameObject platform;

    bool moving;
    bool moved;
    float finalYRotation;

    void Start() {
        moving = false;
        moved = false;
        finalYRotation = transform.eulerAngles.y + 80.0f;
    }

    void Update() {
        if (moved) return;

        if (!moving && platform.GetComponent<Platform_Script>().exitedTrigger) {
            moving = true;
        }

        if (moving) {
            transform.rotation *= Quaternion.Euler(0, 0.6f, 0);

            // Calculate the difference in rotation, taking wrapping into account
            float yRotationDifference = Mathf.DeltaAngle(transform.eulerAngles.y, finalYRotation);

            // Check if the difference is small enough (considering a small threshold to avoid floating point precision issues)
            if (Mathf.Abs(yRotationDifference) < 0.5f) {
                transform.rotation = Quaternion.Euler(transform.eulerAngles.x, finalYRotation, transform.eulerAngles.z);
                moving = false;
                moved = true;
                Platform_Script platformScript = platform.GetComponent<Platform_Script>();
                if (platformScript != null) {
                    platformScript.ActivateCollider();
                }
                else {
                    Debug.LogError("'Platform_Script' not found on the 'platform' GameObject.");
                }
            }
        }
    }
}