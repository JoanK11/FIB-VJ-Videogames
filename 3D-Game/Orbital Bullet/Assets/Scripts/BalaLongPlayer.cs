using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class BalaLongPlayer : BalaBase
{
    // Start is called before the first frame update
    private float Distance;
    public void init() {
        rotationSpeed = -100.0f;
        damage = 30.0f;
        base.initBala();
        Distance = 0.0f;
    }

    // Update is called once per frame
    void Update()
    {
        base.Move();
        Distance += Math.Abs(rotationSpeed * Time.deltaTime);
        if (Distance >= 360.0) Destroy(gameObject);
    }
    protected void OnTriggerEnter(Collider other)
    {
        Debug.Log(other.gameObject.name + " ha entrado en el colider de " + gameObject.name);
       

        if (other.gameObject.tag == "Enemy")
        {
            EnemyBase enemy = other.gameObject.GetComponent<EnemyBase>();
            enemy.takeDamage(GetDamage());
        }

        Destroy(gameObject);
    }
}
