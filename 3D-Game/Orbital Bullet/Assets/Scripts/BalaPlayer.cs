using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BalaPlayer : BalaBase {
    // Start is called before the first frame update

    public void init() {
        rotationSpeed = -10.0f;
        damage = 10.0f;
        initBala();
    }

    // Update is called once per frame
    void FixedUpdate() {

        Move();
    }
    protected void OnTriggerEnter(Collider other) {
        Debug.Log(other.gameObject.name + " ha entrado en el colider de " + gameObject.name);
        MeshRenderer mesh = GetComponent<MeshRenderer>();
        mesh.enabled = false;

        if (other.gameObject.tag == "Enemy") {
            EnemyBase enemy = other.gameObject.GetComponent<EnemyBase>();
            enemy.takeDamage(GetDamage());
        }

        Destroy(gameObject);
    }
    
}