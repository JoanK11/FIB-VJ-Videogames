using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(AudioSource))]
public class PlayerAudio : MonoBehaviour {
    AudioSource audioSource;

    public AudioClip jumpSound1, jumpSound2;
    public AudioClip attackSound;
    public AudioClip damageSound1, damageSound2;
    public AudioClip teleportSound;
    public AudioClip ringChangeSound;
    public AudioClip chestSound;
    public AudioClip noBulletsSound;
    public AudioClip dieSound;
    public AudioClip reloadSound;

    private void Start() {
        audioSource = GetComponent<AudioSource>();
    }

    public void PlayJumpSound() {
        if (audioSource.isPlaying) return;

        int random = Random.Range(0, 2); // [min, max)
        if (random == 0) audioSource.clip = jumpSound1;
        else audioSource.clip = jumpSound2;
        audioSource.Play();
    }

    public void PlayDamageSound() {
        int random = Random.Range(0, 2); // [min, max)
        if (random == 0) audioSource.clip = damageSound1;
        else audioSource.clip = damageSound2;
        audioSource.Play();
    }

    public void PlayTeleportSound() {
        audioSource.clip = teleportSound;
        audioSource.Play();
    }

    public void PlayRingChangeSound() {
        audioSource.clip = ringChangeSound;
        audioSource.Play();
    }

    public void PlayChestSound() {
        audioSource.clip = chestSound;
        audioSource.Play();
    }

    public void PlayAttackSound() {
        audioSource.clip = attackSound;
        audioSource.Play();
    }

    public void PlayNoBulletsSound() {
        if (audioSource.isPlaying) return;

        audioSource.clip = noBulletsSound;
        audioSource.Play();
    }

    public void PlayDieSound() {
        audioSource.clip = dieSound;
        audioSource.Play();
    }

    public void PlayReloadSound() {
        audioSource.clip = reloadSound;
        audioSource.Play();
    }
}