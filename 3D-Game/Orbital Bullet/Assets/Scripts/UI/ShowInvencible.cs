using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class ShowInvencible : MonoBehaviour {
    public MovePlayer player;
    Image img;
    TextMeshProUGUI text;

    void Start() {
        img = GetComponentInChildren<Image>();
        text = GetComponentInChildren<TextMeshProUGUI>();
    }

    // Update is called once per frame
    void Update() {
        bool playerInvencible = player.IsInvencible();
        img.enabled = playerInvencible;
        text.enabled = playerInvencible;
    }
}
