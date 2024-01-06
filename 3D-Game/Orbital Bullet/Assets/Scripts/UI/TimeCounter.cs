using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro; // Make sure to include the TextMeshPro namespace

public class TimeCounter : MonoBehaviour {
    float startTime;
    TextMeshProUGUI timeText;

    void Start() {
        startTime = Time.time;
        timeText = GetComponent<TextMeshProUGUI>();
    }

    void Update() {
        // Calculate the time elapsed since the game started
        float timeSinceStart = Time.time - startTime;

        // Convert the time to minutes and seconds
        string minutes = ((int)timeSinceStart / 60).ToString("00");
        string seconds = (timeSinceStart % 60).ToString("00");

        // Update the TextMeshPro text to show the time elapsed
        timeText.text = minutes + ":" + seconds;
    }
}
