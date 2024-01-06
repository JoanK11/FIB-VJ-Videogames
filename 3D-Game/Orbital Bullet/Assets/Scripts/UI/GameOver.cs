using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameOver : MonoBehaviour {
    public BossLevel bossLevel;

    public void OnPlayerDeath() {
        gameObject.SetActive(true);
        Time.timeScale = 0;
        bossLevel.PauseMusic();
    }

    public void Retry() {
        gameObject.SetActive(false);
        Time.timeScale = 1;
        SceneManager.LoadScene(1);
    }

    public void LoadMenu() {
        gameObject.SetActive(false);
        Time.timeScale = 1;
        SceneManager.LoadScene(0);
    }

    public void QuitGame() {
        Application.Quit();
    }
}
