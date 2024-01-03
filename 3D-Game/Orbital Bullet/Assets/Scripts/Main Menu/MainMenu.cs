using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

[RequireComponent(typeof(AudioSource))]
public class MainMenu : MonoBehaviour {
    AudioSource audioSource;
    public AudioClip acceptSound;
    public AudioClip cancelSound;

    void Start() {
        audioSource = GetComponent<AudioSource>();
    }

    public void PlayGame() {
        SceneManager.LoadSceneAsync(1);
    }

    public void QuitGame() {
        Application.Quit();
    }

    public void PlayAcceptSound() {
        audioSource.clip = acceptSound;
        audioSource.Play();
    }

    public void PlayCancelSound() {
        audioSource.clip = cancelSound;
        audioSource.Play();
    }
}
