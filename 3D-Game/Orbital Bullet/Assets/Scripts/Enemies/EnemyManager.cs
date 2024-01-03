using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyManager : MonoBehaviour {
    public int enemyCount;
    public Jump jumpScript;

    void Start() {
        enemyCount = transform.childCount;
    }

    public void EnemyDefeated() {
        enemyCount--;
        if (enemyCount == 0) {
            jumpScript.UpdateUI();
        }
    }
}
