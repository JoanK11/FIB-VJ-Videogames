using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BossEnemy : EnemyBase {
    public float rotationSpeed, gravity;
    float rotationSpeedAssault;
    Vector3 startDirection;
    float speedY;

    Quaternion originalrotation;
    bool isRight;

    CharacterController charControl;
    Vector3 reference;
    enum State {GOOD, ASSSAULT}
    State currentState;
    const float timeAssault = 4.5f;
    const float timeGood = 5.0f;
    float time;

    const float timeAttack = 4.0f;
    float timeA;
    bool canAttack;
    public GameWin gameWin;
    public BossLevel bossLevel;
    public GameObject Bar;
    void Start() {
        reference = new Vector3(50, 0, 0);
        Bar.SetActive(true);
        // Store starting direction of the player with respect to the axis of the level
        startDirection = transform.position - reference;
        startDirection.y = 0.0f;
        startDirection.Normalize();
        speedY = 0;

        originalrotation = transform.rotation;
        isRight = true;

        charControl = GetComponent<CharacterController>();
        charControl.detectCollisions = true;

        base.init();

        currentState = State.GOOD;
        gravity = 1;
        rotationSpeed = -20.0f;
        rotationSpeedAssault = -60.0f;
        time = 0;
        canAttack = true;
        timeA = 0;
    }

    void FixedUpdate() {
        // Destroy the enemy if it has died and finished making the sound
        if (playedSound && !audioSource.isPlaying) {
            Destroy(gameObject);
            if (gameObject.name == "Boss") {
                Debug.Log("He entrado en la pantalla de win");
                gameWin.OnGameWin();
                bossLevel.WinGame();
            }
            return;
        }

        if (playedSound) return;

        if (!canAttack) {
            timeA += Time.deltaTime;
            if (timeA >= timeAttack) canAttack = true;
        }

        if (currentState == State.GOOD) {
            time += Time.deltaTime;
            Vector3 position = transform.position;

            float angle = rotationSpeed * Time.deltaTime;
            Vector3 direction = position - reference;
            Vector3 target;

            // Left-right movement
            target = reference + Quaternion.AngleAxis(angle, Vector3.up) * direction;


            if (charControl.Move(target - position) == CollisionFlags.Sides) {
                transform.position = position;
                rotationSpeed *= -1;
                rotationSpeedAssault *= -1;
                isRight = !isRight;

            }
            if (time >= timeGood) {
                currentState = State.ASSSAULT;
                time = 0;
            }
        }
        else if (currentState == State.ASSSAULT){
            time += Time.deltaTime;
            Vector3 position = transform.position;

            float angle = rotationSpeedAssault * Time.deltaTime;
            Vector3 direction = position - reference;
            Vector3 target;

            // Left-right movement
            target = reference + Quaternion.AngleAxis(angle, Vector3.up) * direction;
            if (charControl.Move(target - position) == CollisionFlags.Sides) {
                //Debug.Log("Ha entrado el boosss2");
                transform.position = position;
                rotationSpeed *= -1;
                rotationSpeedAssault *= -1;
                isRight = !isRight; ;

            }
            if (time >= timeAssault) {
                currentState = State.GOOD;
                rotationSpeed *= -1;
                rotationSpeedAssault *= -1;
                time = 0;
            }
        }

        // Correct orientation of player
        // Compute current direction
        Vector3 currentDirection = transform.position - reference;
        currentDirection.y = 0.0f;
        currentDirection.Normalize();
        // Change orientation of player accordingly
        Quaternion orientation;
        if ((startDirection - currentDirection).magnitude < 1e-3) {
            orientation = Quaternion.AngleAxis(0.0f, Vector3.up);
        }
        else if ((startDirection + currentDirection).magnitude < 1e-3) {
            orientation = Quaternion.AngleAxis(180.0f, Vector3.up);
        }
        else {
            orientation = Quaternion.FromToRotation(startDirection, currentDirection);
        }
        transform.rotation = orientation * originalrotation;

        if (!isRight) transform.rotation *= Quaternion.Euler(0, 180, 0);

        Vector3 pos = transform.position;
        if (charControl.Move(speedY * Time.deltaTime * Vector3.up) != CollisionFlags.None) {
            transform.position = pos;
            Physics.SyncTransforms();
        }

        if (charControl.isGrounded) {
            if (speedY < 0.0f) {
                speedY = 0.0f;
            }
        } else {
            speedY -= gravity * Time.deltaTime;
        }

        lookCamera();
    }

    void OnControllerColliderHit(ControllerColliderHit hit) {
        // if (hit.gameObject.tag == "Floor") return;
        //Debug.Log("he hiteado a " + hit.gameObject.name);
        if (canAttack && hit.gameObject.tag == "Player") {
            canAttack = false;
            timeA = 0;
            if (currentState == State.GOOD) hit.gameObject.GetComponent<MovePlayer>().TakeDamage(20);
            else if (currentState == State.ASSSAULT) hit.gameObject.GetComponent<MovePlayer>().TakeDamage(40);
        }
    }
}
