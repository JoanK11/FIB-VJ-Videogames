using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyManager : MonoBehaviour {
    public int enemyCount;
    public Jump jumpScript;
    GameObject player;

    void Start() {
        enemyCount = transform.childCount;
        player = GameObject.Find("Player");
    }

    public void EnemyDefeated() {
        enemyCount--;
        player.GetComponent<MovePlayer>().IncreaseAmmo(3);

        if (enemyCount == 0) {
            jumpScript.UpdateUI();
        }
    }
}
