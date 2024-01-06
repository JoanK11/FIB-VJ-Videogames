using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class World : MonoBehaviour {
    int level;

    /* -- Game Objects -- */
    public MovePlayer player;
    public FollowPlayer camera;
    public BossLevel bossLevel;

    void Start() {
        level = 1;
    }

    void Update() {
        if (Input.GetKeyDown(KeyCode.Alpha2) && level < 2) {
            SetLevel2();
            level = 2;
        }
        else if (Input.GetKeyDown(KeyCode.Alpha3) && level < 3) {
            SetLevel3();
            level = 3;
        }
        else if (Input.GetKeyDown(KeyCode.Alpha4) && level < 4) {
            SetLevel4();
            level = 4;
        }
        else if (Input.GetKeyDown(KeyCode.Alpha6) && level < 6) {
            SetBossLevel();
            level = 6;
        }
    }

    void SetLevel2() {
        /* -- Level -- */
        GameObject level2E = GameObject.Find("Platform_Exterior2");
        level2E.GetComponent<BoxCollider>().isTrigger = false;
        GameObject level2I = GameObject.Find("Platform_Moving2");
        level2I.transform.rotation = Quaternion.Euler(level2I.transform.eulerAngles.x,
                                     135f, level2I.transform.eulerAngles.z);

        /* -- Player -- */
        Vector3 playerPosition = new Vector3(-4.79f, 7f, -6.74f);
        Vector3 newCenter = new Vector3(0, 0, 0);
        player.TeleportTo(playerPosition, newCenter);
    }

    void SetLevel3() {
        /* -- Level -- */
        GameObject level3E = GameObject.Find("Platform_Exterior3");
        level3E.GetComponent<BoxCollider>().isTrigger = false;
        GameObject level3I = GameObject.Find("Platform_Moving3");
        level3I.transform.rotation = Quaternion.Euler(level3I.transform.eulerAngles.x,
                                     level3I.transform.eulerAngles.y + 80.0f, level3I.transform.eulerAngles.z);

        /* -- Player -- */
        Vector3 playerPosition = new Vector3(-6.41f, 12.91f, 5.22f);
        Vector3 newCenter = new Vector3(0, 0, 0);
        player.TeleportTo(playerPosition, newCenter);
    }

    void SetLevel4() {
        /* -- Player -- */
        Vector3 playerPosition = new Vector3(39.3f, 12.95f, -0.45f);
        Vector3 newCenter = new Vector3(50, 0, 0);
        player.TeleportTo(playerPosition, newCenter);

        /* -- Camera -- */
        if (level < 4) {
            Quaternion newRotation = new Quaternion(0, -0.72176f, 0, -0.69214f);
            Vector3 newPosition = new Vector3(50f, 0, 0);
            camera.SetCamera(newRotation, newPosition, newCenter);
        }
    }

    void SetLevel5() {
        /* -- Player -- */
        Vector3 playerPosition = new Vector3(55.6f, 18.95f, 2.93f);
        Vector3 newCenter = new Vector3(50, 0, 0);
        player.TeleportTo(playerPosition, newCenter);

        /* -- Camera -- */
        if (level < 4) {
            Quaternion newRotation = new Quaternion(0, -0.72176f, 0, -0.69214f);
            Vector3 newPosition = new Vector3(50f, 0, 0);
            camera.SetCamera(newRotation, newPosition, newCenter);
        }
    }

    void SetBossLevel() {
        /* -- Level -- */
        bossLevel.SetBossLevel();

        /* -- Player -- */
        Vector3 playerPosition = new Vector3(42.7f, 51.41f, 0.4f);
        Vector3 newCenter = new Vector3(50, 0, 0);
        player.TeleportTo(playerPosition, newCenter);

        /* -- Camera -- */
        if (level < 4) {
            Quaternion newRotation = new Quaternion(0, -0.72176f, 0, -0.69214f);
            Vector3 newPosition = new Vector3(50f, 0, 0);
            camera.SetCamera(newRotation, newPosition, newCenter);
        }
    }
}
