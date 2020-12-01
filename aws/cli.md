## Install & Configure

```
pip install awscli
aws configure
aws configure --profile <profile name>

ls ~/.aws/
```

## Assume Role

TODO

## Filter 

```
aws ec2 describe-instances --filter Name=xx,State=yy
```

## Query & Jmespath

```
aws ec2 esecribe-instances --query <jmespath> 
```

## generate cli-skeleton

```
aws ec2 run-instances --generate-cli-skeleton
```

## S3

```
aws s3 copy <location> <location>
aws s3 sync <location> <location>
```