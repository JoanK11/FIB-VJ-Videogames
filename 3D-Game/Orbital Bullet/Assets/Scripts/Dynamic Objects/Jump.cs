using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro; // TextMesh Pro namespace
using UnityEngine.UI; // Image namespace

public class Jump : MonoBehaviour {
    public GameObject player;
    public World world;
    protected bool isPlayerOnTrigger;

    /* -- Enemy Manager -- */
    public EnemyManager enemyManager;

    /* -- UI -- */
    public GameObject UIButton;
    protected TextMeshProUGUI UItext;
    protected Image UIimage;
    protected Sprite keySprite;

    protected void Start() {
        isPlayerOnTrigger = false;

        player = GameObject.Find("Player");

        UItext = UIButton.GetComponentInChildren<TextMeshProUGUI>();
        UIimage = UIButton.GetComponentInChildren<Image>();
        keySprite = Resources.Load<Sprite>("E-Key");
    }

    protected virtual void Update() {
        if (isPlayerOnTrigger && Input.GetKeyUp(KeyCode.E) && enemyManager.enemyCount == 0) {
            player.GetComponent<MovePlayer>().JumpNextLevel();
            world.IncreaseLevel();
            Debug.Log(name + ": Player jumped to the next level.");
        }
    }

    protected void OnTriggerEnter(Collider other) {
        if (other.gameObject == player) {
            isPlayerOnTrigger = true;
            ShowUI(true);
            Debug.Log(name + ": Player entered Trigger.");
        }
    }

    protected void OnTriggerExit(Collider other) {
        if (other.gameObject == player) {
            isPlayerOnTrigger = false;
            ShowUI(false);
            Debug.Log(name + ": Player exited Trigger.");
        }
    }

    protected virtual void ShowUI(bool show) {
        if (show) {
            if (enemyManager.enemyCount > 0) {
                UItext.text = "Defeat all enemies to access the next level!";
                UItext.color = Color.red;
                UIimage.gameObject.SetActive(false);
            }
            else {
                UItext.text = "Jump to the next sector";
                UItext.color = Color.white;
                UIimage.sprite = keySprite;
                UIimage.gameObject.SetActive(true);
            }
            UIButton.SetActive(true);
        } else {
            UIimage.gameObject.SetActive(true);
            UIButton.SetActive(false);
        }
    }

    public void UpdateUI() {
        ShowUI(isPlayerOnTrigger);
    }
}
