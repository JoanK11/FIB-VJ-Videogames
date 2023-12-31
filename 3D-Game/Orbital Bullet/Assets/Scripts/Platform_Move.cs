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

        if (!moving && platform.GetComponent<Platform_Script>().enteredTrigger) {
            moving = true;
        }

        if (moving) {
            transform.rotation *= Quaternion.Euler(0, 1.5f, 0);

            // Calculate the difference in rotation, taking wrapping into account
            float yRotationDifference = Mathf.DeltaAngle(transform.eulerAngles.y, finalYRotation);

            // Check if the difference is small enough
            if (Mathf.Abs(yRotationDifference) < 1.5f) {
                transform.rotation = Quaternion.Euler(transform.eulerAngles.x, finalYRotation, transform.eulerAngles.z);
                moving = false;
                moved = true;
            }
        }
    }
}