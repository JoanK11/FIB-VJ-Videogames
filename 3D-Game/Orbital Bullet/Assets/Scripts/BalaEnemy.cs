using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BalaEnemy : BalaPlayer
{
    // Start is called before the first frame update
    public void init() {
        rotationSpeed = -100.0f;
        damage = 10.0f;
        initBala();
    }

    // Update is called once per frame
    void FixedUpdate()
    {

        Move();
    }
    protected void OnTriggerEnter(Collider other)
    {
        Debug.Log(other.gameObject.name + " ha entrado en el colider de " + gameObject.name);
        MeshRenderer mesh = GetComponent<MeshRenderer>();
        mesh.enabled = false;

        if (other.gameObject.tag == "Player")
        {
            MovePlayer player = other.gameObject.GetComponent<MovePlayer>();
            //player.takeDamage(GetDamage());
        }

        Destroy(gameObject);
    }
}
