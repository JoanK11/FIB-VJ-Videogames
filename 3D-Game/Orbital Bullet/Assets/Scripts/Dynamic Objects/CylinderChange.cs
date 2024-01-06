using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro; // TextMesh Pro namespace
using UnityEngine.UI; // Image namespace

public class CylinderChange : Jump {
    public GameObject target;

    protected override void Update() {
        if (isPlayerOnTrigger && Input.GetKeyUp(KeyCode.E) && enemyManager.enemyCount == 0) {
            Vector3 targetPosition = target.transform.position;
            Debug.Log(name + ": Player needs to go to " + targetPosition);
            world.IncreaseLevel();
            player.GetComponent<MovePlayer>().ChangeCylinder(targetPosition);
        }
    }

    protected override void ShowUI(bool show) {
        if (show) {
            if (enemyManager.enemyCount > 0) {
                UItext.text = "Defeat all enemies to switch cylinder!";
                UItext.color = Color.red;
                UIimage.gameObject.SetActive(false);
            }
            else {
                UItext.text = "Switch Cylinder";
                UItext.color = Color.white;
                UIimage.sprite = keySprite;
                UIimage.gameObject.SetActive(true);
            }
            UIButton.SetActive(true);
        }
        else {
            UIimage.gameObject.SetActive(true);
            UIButton.SetActive(false);
        }
    }
}


