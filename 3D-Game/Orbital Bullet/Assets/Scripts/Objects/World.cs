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
        if (Input.GetKeyDown(KeyCode.Alpha6) && level != 6) {
            level = 6;
            SetBossLevel();
        }
    }

    void SetBossLevel() {
        /* -- Level -- */
        bossLevel.SetBossLevel();

        /* -- Player -- */
        Vector3 playerPosition = new Vector3(39f, 51.5f, 0.37f);
        Vector3 newCenter = new Vector3(50, 0, 0);
        player.TeleportTo(playerPosition, newCenter);

        /* -- Camera -- */
        Quaternion newRotation = new Quaternion(0, -0.72176f, 0, -0.69214f);
        Vector3 newPosition = new Vector3(50f, 0, 0);
        camera.SetCamera(newRotation, newPosition, newCenter);
    }

}
