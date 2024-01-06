using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(AudioSource))]
public class BossLevel : MonoBehaviour {
    float rotationSpeed = 25.0f;
    public bool exitedTrigger;
    public GameObject boss;

    /* -- Music -- */
    AudioSource audioSource;
    public AudioClip bossMusic;

    void Start() {
        exitedTrigger = false;

        /* -- Music -- */
        audioSource = GetComponent<AudioSource>();
        audioSource.clip = bossMusic;
    }

    void FixedUpdate() {
        if (exitedTrigger) {
            transform.Rotate(0, rotationSpeed * Time.deltaTime, 0);
        }
    }

    void OnTriggerExit(Collider other) {
        if (other.gameObject.CompareTag("Player")) {
            other.GetComponent<MovePlayer>().ArrivedNextLevel();
            SetBossLevel();
        }
    }

    public void SetBossLevel() {
        exitedTrigger = true;
        GetComponent<BoxCollider>().isTrigger = false;
        audioSource.Play();
        boss.SetActive(true);
    }

    public void WinGame() {
        audioSource.Stop();
    }
}
