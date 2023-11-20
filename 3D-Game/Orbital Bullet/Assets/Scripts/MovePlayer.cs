using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MovePlayer : MonoBehaviour
{
    public float moveSpeed = 3.0f, jumpSpeed = 5.0f;
    public float gravity = 10.0f;

    public AudioClip jumpSound;

    int orientation = 1;

    Vector2 speed;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKey(KeyCode.A) || Input.GetKey(KeyCode.LeftArrow))
        {
            if (orientation == 1)
                transform.Rotate(0.0f, 180.0f, 0.0f);
            speed.x = -moveSpeed;
            orientation = 0;
        }
        else if (Input.GetKey(KeyCode.D) || Input.GetKey(KeyCode.RightArrow))
        {
            if (orientation == 0)
                transform.Rotate(0.0f, 180.0f, 0.0f);
            speed.x = moveSpeed;
            orientation = 1;
        }
        else
            speed.x = 0.0f;

        if ((speed.y < 0) && GetComponent<CharacterController>().isGrounded)
            speed.y = 0.0f;
        if (Input.GetKey(KeyCode.W) && GetComponent<CharacterController>().isGrounded)
        {
            AudioSource.PlayClipAtPoint(jumpSound, transform.position);
            speed.y = jumpSpeed;
        }

        speed.y -= gravity * Time.deltaTime;

        Vector2 displace = speed * Time.deltaTime;
        GetComponent<CharacterController>().Move(new Vector3(displace.x, displace.y, 0.0f));
    }
}
