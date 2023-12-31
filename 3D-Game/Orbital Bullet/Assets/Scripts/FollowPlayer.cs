using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FollowPlayer : MonoBehaviour {
    GameObject player;
    Vector3 startDirection;
    float yOffset;
    bool changingCylinder;
    Vector3 Center;


    void Start() {
        // Initialize GameObjects
        InitializeGameObjects();

        // Store starting direction of the player with respect to the axis of the level
        startDirection = player.transform.position - player.transform.parent.position;
        startDirection.y = 0.0f;
        startDirection.Normalize();

        // Calculate initial Y offset between camera and player
        yOffset = transform.position.y - player.transform.position.y;

        changingCylinder = false;
        Center = new Vector3(0, 0, 0);
    }

    void Update() {
        if (changingCylinder) return;
        // Update direction and orientation
        UpdateDirectionAndOrientation();

        // Update camera's Y position to follow player's Y position
        FollowPlayerYPosition();
    }

    private void InitializeGameObjects() {
        player = GameObject.Find("Player");

        if (player == null) {
            Debug.LogError(name + ": Player object not found. Make sure your player is named 'Player'.");
        }
    }

    private void UpdateDirectionAndOrientation() {
        // Compute current direction
        Vector3 currentDirection = player.transform.position - Center;
        currentDirection.y = 0.0f;
        currentDirection.Normalize();

        // Change orientation of the camera pivot to match the player's
        Quaternion orientation;
        if ((startDirection - currentDirection).magnitude < 1e-3) {
            orientation = Quaternion.AngleAxis(0.0f, Vector3.up);
        }
        else if ((startDirection + currentDirection).magnitude < 1e-3) {
            orientation = Quaternion.AngleAxis(180.0f, Vector3.up);
        }
        else {
            orientation = Quaternion.FromToRotation(startDirection, currentDirection);
        }
        transform.parent.rotation = orientation;
    }

    private void FollowPlayerYPosition() {
        Vector3 cameraPosition = transform.position;
        cameraPosition.y = player.transform.position.y + yOffset;
        transform.position = cameraPosition;
    }

    public IEnumerator ChangeCylinder(float duration, float moveAmountX) {
        float elapsed = 0.0f;
        changingCylinder = true;

        // Use the parent's rotation as the starting point
        Quaternion startRotation = transform.parent.rotation;
        Vector3 euler = startRotation.eulerAngles;
        euler.y = -euler.y;
        Quaternion endRotation = Quaternion.Euler(euler);

        Vector3 startPosition = transform.parent.position;
        Vector3 endPosition = startPosition + new Vector3(moveAmountX, 0, 0);
        float durationRotation = duration / 3;

        while (elapsed < duration) {
            elapsed += Time.deltaTime;
            float progress = elapsed / duration;
            float progressRotation = elapsed / durationRotation;

            // Rotate the parent of the camera
            transform.parent.rotation = Quaternion.Lerp(startRotation, endRotation, progressRotation);

            // Move the parent of the camera on the X axis
            transform.parent.position = new Vector3(
                Mathf.Lerp(startPosition.x, endPosition.x, progress),
                transform.parent.position.y,
                transform.parent.position.z
            );

            yield return null;
        }

        // Ensures the parent is exactly at the final rotation and position
        transform.parent.rotation = endRotation;
        transform.parent.position = endPosition;
        
        // Update the center and direction accordingly
        Center = new Vector3(50, 0, 0);
        changingCylinder = false;
    }
}
