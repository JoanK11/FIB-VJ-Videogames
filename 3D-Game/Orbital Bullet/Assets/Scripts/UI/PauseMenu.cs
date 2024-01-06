using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PauseMenu : MonoBehaviour {
    public static bool GamePaused = false;
    public BossLevel bossLevel;
    public GameObject PauseMenuUI;

    // Update is called once per frame
    void Update() {
        if (Input.GetKeyDown(KeyCode.Escape)) {
            if (GamePaused) {
                Resume();
            }
            else {
                Pause();
            }
        }
    }

    public void Resume() {
        PauseMenuUI.SetActive(false);
        Time.timeScale = 1;
        GamePaused = false;
        bossLevel.ResumeMusic();
    }

    void Pause() {
        PauseMenuUI.SetActive(true);
        Time.timeScale = 0;
        GamePaused = true;
        bossLevel.PauseMusic();
    }

    public void LoadMenu() {
        Time.timeScale = 1;
        SceneManager.LoadScene(0);
    }

    public void QuitGame() {
        Application.Quit();
    }
}
