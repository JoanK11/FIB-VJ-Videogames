using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class BalaPlayer : BalaBase {
    // Start is called before the first frame update
    GameObject prefab;
    public void init() {
        rotationSpeed = -10.0f;
        damage = 10.0f;
        base.initBala();
        
        prefab = AssetDatabase.LoadAssetAtPath<GameObject>("Assets/Prefabs/SmallExplosion.prefab");
        Center = GameObject.Find("Player").GetComponent<MovePlayer>().GetCenter();
        Quaternion pre = AssetDatabase.LoadAssetAtPath<GameObject>("Assets/Prefabs/Bala.prefab").transform.rotation;
        rotacionInicial = GameObject.Find("Player").transform.rotation * Quaternion.Euler(0, 90, 0) * pre;
  
    }

    // Update is called once per frame
    void FixedUpdate() {

        Move();
    }
    protected void OnTriggerEnter(Collider other) {
        Debug.Log(other.gameObject.name + " ha entrado en el colider de " + gameObject.name);
        

        if (other.gameObject.tag == "Enemy") {
            EnemyBase enemy = other.gameObject.GetComponent<EnemyBase>();
            enemy.takeDamage(GetDamage());
        }
        GameObject explosion = Instantiate(prefab, transform.position, Quaternion.identity);
        Destroy(explosion, 1.0f);
        Destroy(gameObject);
    }
    
}