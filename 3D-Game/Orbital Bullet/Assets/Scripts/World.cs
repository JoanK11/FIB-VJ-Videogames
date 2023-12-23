using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class World : MonoBehaviour {
    public int level;

    void Start() {
        level = 1;
    }

    void Update() {

    }

    public void NextLevel() {
        level++;
    }
}
