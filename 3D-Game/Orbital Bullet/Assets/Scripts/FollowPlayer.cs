using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FollowPlayer : MonoBehaviour {
    public GameObject player;
    public GameObject world;

    public int cameraLevel;
    Vector3 startDirection;

    void Start() {
        // Store starting direction of the player with respect to the axis of the level
        startDirection = player.transform.position - player.transform.parent.position;
        startDirection.y = 0.0f;
        startDirection.Normalize();

        cameraLevel = 1;

        /* GameObjects initializations */
        player = GameObject.Find("Player");
        if (player == null)
            Debug.LogError("Player object not found. Make sure your player is named 'Player'.");

        world = GameObject.Find("World");
        if (player == null)
            Debug.LogError("World object not found. Make sure your world is named 'World'.");
    }

    void Update() {
        // Compute current direction
        Vector3 currentDirection = player.transform.position - player.transform.parent.position;
        currentDirection.y = 0.0f;
        currentDirection.Normalize();

        // Check for level change
        int worldLevel = world.GetComponent<World>().level;
        if (worldLevel != cameraLevel) {
            StartCoroutine(MoveCameraUpwards()); 
            cameraLevel++;
        }

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

    IEnumerator MoveCameraUpwards() {
        float duration = 2.0f; // Duration of the transition in seconds
        Vector3 startPosition = transform.position;
        Vector3 endPosition = new Vector3(startPosition.x, startPosition.y + 5.0f, startPosition.z);

        float elapsedTime = 0;
        while (elapsedTime < duration) {
            transform.position = Vector3.Lerp(startPosition, endPosition, elapsedTime / duration);
            elapsedTime += Time.deltaTime;
            yield return null;
        }

        transform.position = endPosition; // Ensure it ends at the exact position
    }
}
