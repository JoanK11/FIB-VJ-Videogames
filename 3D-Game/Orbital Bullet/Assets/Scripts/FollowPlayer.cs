using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FollowPlayer : MonoBehaviour {
    private GameObject player;
    private Vector3 startDirection;
    private float yOffset;

    void Start() {
        // Initialize GameObjects
        InitializeGameObjects();

        // Store starting direction of the player with respect to the axis of the level
        startDirection = player.transform.position - player.transform.parent.position;
        startDirection.y = 0.0f;
        startDirection.Normalize();

        // Calculate initial Y offset between camera and player
        yOffset = transform.position.y - player.transform.position.y;
    }

    void Update() {
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
        Vector3 currentDirection = player.transform.position - player.transform.parent.position;
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
}
